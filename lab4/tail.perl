#!/usr/bin/perl -T

use strict;
use warnings;

my $isReverse = 0;
my $processingFileNames = 0;
my $isInteractive = 0;
my $isOffsetMode = 0;
my $processNumber = 0;
my $linesLimit = 10;

my %keys;

my @files;

foreach (@ARGV) {

    unless ($processingFileNames || exists $keys{$_}) {
        if ($processNumber) {
            if ($_ =~ /^[1-9]\d*$/) {
                $linesLimit = $_;
                $processNumber = 0;
            } elsif ($_ =~ /^\+[1-9]\d*$/) {
                $linesLimit = $_;
                $processNumber = 0;
                $linesLimit =~ s/^\+([1-9]\d*)$/$1/;
                $isOffsetMode = 1;
            } else {
                startProcessingFileNames("-n", $_);
            }
        } else {
            if ($_ eq "-n") {
                $keys{$_} = 1;
                $processNumber = 1;
            } elsif ($_ eq "-f") {
                if ($isReverse) {
                    startProcessingFileNames($_);
                } else {
                    $isInteractive = 1;
                    $keys{$_} = 1;
                }
            } elsif ($_ eq "-r") {
                if ($isInteractive) {
                    startProcessingFileNames($_) if $isInteractive;
                } else {
                    $isReverse = 1;
                    $keys{$_} = 1;
                }
            } elsif ($_ eq "--") {
                $processingFileNames = 1;
            } else {
                startProcessingFileNames($_);
            }
        }
    } else {
        startProcessingFileNames($_);
    }

}

my $in;

if (scalar(@files)) {
  foreach (@files) {
    open($in, $_) or die "Can't open $_ for input: $!\n";

    print "==> $_ <==\n" if scalar(@files) > 1;

    processFile();
  }
} else {
  $in = *STDIN;

  processFile();
}

sub processFile {
  my @buf;
  my $linesCounter = 0;

  while (<$in>) {
    if ($linesCounter < $linesLimit) {
      $linesCounter++;

      next if ($isOffsetMode);
    } else {
      shift @buf unless ($isOffsetMode);
    }

    push @buf, $_;
  }
  close $in;

  printArray(@buf);
}

sub printArray {
  if ($isReverse) {
    print reverse @_;
  } else {
    print @_;
  }
}

sub startProcessingFileNames {
  $processingFileNames = 1;
  push @files, @_;
}
