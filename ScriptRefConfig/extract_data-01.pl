#!/usr/bin/perl
# Extract data from Doug's "Daily Update" emails
# Brett --- Feb 2023
use v5.10;
use strict;
use warnings;
use File::Basename;

say "# Reads by chunk, prints every number present as a row";

say "# Test Data from Doug Arlt's 'Daily Update' emails";
open my $fh, $ARGV[0] or die "$!";
print "# ", basename($ARGV[0]), "\n";
local $/ = 'From:doug_arlt\@selinc.com'; # read each day's email as a chunk
while (my $chunk = <>) {
    chomp $chunk;
    my $date = $chunk =~ /^Sent:.*, (\.*?)$/;
    say $date;
    my @fields = ($chunk =~ /(\d+\.?\d*%?)/g);
    next unless @fields;
    print join(' ', @fields), "\n";
}
close $fh;
