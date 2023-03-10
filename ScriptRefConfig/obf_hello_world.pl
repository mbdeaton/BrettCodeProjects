#!/usr/bin/perl
# Obfuscated "Hello World" for Culture of Learning Code Challenge
# Brett Deaton --- Mar 2023

use v5.10;
use strict;
use warnings;
use Getopt::Long;
use LWP::Simple;

sub print_help { print <<EOF;
Print the phrase commonly used to introduce a new programming language.
  -h, --help    print this help text
  -n <int>      find words with length n (default 5)
  -m <int>      print the m most-frequent words (default 2)
EOF
}

sub find_it {
    my ($n, $m) = @_; # destructure args
    my $url = "http://tutorials.github.io/pages/hello-world.html";
    my $page = get($url) or die "can't retrieve url";

    # capture n-letter words
    my @words = ($page =~ m/[^a-z]([d-w]{\Q$n\E})[^a-z]/ig);

    # create a frequency table of the words, case-insensitive
    my %word_freq;
    foreach my $word (@words) {
	$word_freq{lc $word}++;
    }

    # sort by frequency, see https://perldoc.perl.org/functions/sort
    my @words_sorted = sort { $word_freq{$b} <=> $word_freq{$a} } keys %word_freq;

    # print it!
    my ($say_sep, $say_end) = (", ", "!");
    say join($say_sep, @words_sorted[0..($m-1)]), $say_end;
}

GetOptions(
    "help|h" => \( my $help = undef ),
    "n=i"      => \( my $n = 5 ),
    "m=i"      => \( my $m = 2 ),
    )
    or die ("Error in command line args");

if ( $help ) { print_help(); }
else { find_it($n, $m); }
