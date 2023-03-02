#!/usr/bin/perl
# Extract data from Doug's "Daily Update" emails
# Brett --- Feb 2023
use v5.10;
use strict;
use warnings;
use File::Basename;
use Getopt::Long;
use Time::Piece;

my $help_text = <<EOF;
Extract test data from Doug Arlt's daily emails. Print to stdout.
Usage: extract.pl [--help] [--nstop N] FILENAME
  -h, --help    print this help text
  -n, --nstop   stop scanning emails after N data rows (def: scan all)
  -s, --sep     separator character (def: " ")
EOF

sub print_help { print $help_text; }

sub print_header {
    my @fields = (
	"date",
	"julianday",
	"station_usage",
	"total_units",
	"passing_units",
	"units_with_failures_aborts",
	"total_failures_aborts",
	"avg_success_time",
	"avg_fail_time",
	"avg_idle_time",
	"first_pass_yield",
	"capacity_used",
	"capacity_used_on_failed_tests",
	"daily_unit_goal",
	"max_potential_units",
	);
    for (my $i=0; $i<$#fields; $i++) {
	say "# [$i] $fields[$i]";
    }
}

sub extract_data {
    my ($filename, $rows_to_print, $sep) = @_; # destructure args
    open my $fh, $filename or die "$!";
    say "# ", basename($filename);
    print_header();
    local $/ = 'From:'; # read each day's email as a chunk
    my $head = <$fh>; # discard first chunk
    while (my $chunk = <$fh>) {
	if ($rows_to_print and ($.-1 > $rows_to_print)) { last; }
	$chunk =~ m/Sent:\s*(.*)\r\n/; # remove CRLF explicitly, bc $/ was reset
	my $date = $1;
	# strptime uses FreeBSD strftime spec
	my $time = Time::Piece->strptime($date, "%A, %B %e, %Y %I:%M %p");
	print $time->ymd, $sep, int($time->julian_day);
	$chunk =~ m/Daily Results([\w\W]*)Trend Data/; # \w\W instead of . to match \n
	my $subchunk = $1;
	my @data = ($subchunk =~ m/(\d+\.?\d*%?)/g);
	say $sep, join($sep, @data);
    }
    close $fh;
}

GetOptions(
    "help|h"    => \( my $help = undef ),
    "nstop|n=i" => \( my $nstop = undef),
    "sep|s=s"   => \( my $sep = " "),
    )
    or die ("Error in command line args");

if ( $help ) {
    print_help();
} else {
    if ($#ARGV != 0) {
	die ("You must provide a single filename on the command line");
    }
    my $filename = $ARGV[0];
    extract_data( $ARGV[0], $nstop, $sep);
}
