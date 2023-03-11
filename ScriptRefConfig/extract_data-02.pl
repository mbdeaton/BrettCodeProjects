#!/usr/bin/perl
# Extract data from Doug's "Daily Update" emails
# Brett --- Feb 2023
use v5.10;
use strict;
use warnings;
use File::Basename;
use Time::Piece;

say "# Reads by chunk, prints dates on each row";

say "# Test Data from Doug Arlt's 'Daily Update' emails";
open my $fh, $ARGV[0] or die "$!";
say "# ", basename($ARGV[0]);
local $/ = 'From:'; # read each day's email as a chunk
my $display_length = 2; # DEBUG
my $head = <$fh>; # discard first chunk
while (my $chunk = <$fh>) {
    if ($.-1 > $display_length) { last; }
    $chunk =~ m/Sent:\s*(.*)\r\n/; # remove CRLF explicitly since $/ reset
    my $date = $1;
    my $time = Time::Piece->strptime($date, "%A, %B %e, %Y %I:%M %p"); # FreeBSD strftime
    print ($time->ymd);
    say "";
}
close $fh;
