#!/usr/bin/perl
#
# FILE             $Id: $
#
# DESCRIPTION
#
# PROJECT
#
# AUTHOR           Milan Dunghubel <milan.dunghubel@firma.seznam.cz>
#
# Copyright (C) Seznam.cz a.s. 2010
# All Rights Reserved
#
# HISTORY
#       2010/12/27 (dunghubel)
#                  First draft.
#
#use strict;
use warnings;

use CDB_File;
use Getopt::Long;
use Pod::Usage;

use Smart::Comments;

my $output = 'a.cdb';
my $verbose = '';
my $debug = '';

## Parse options and print usage if there is a syntax error,
## or if usage was explicitly requested.
GetOptions('help|h|?' => sub { pod2usage(-verbose => 1) },
           man => sub { pod2usage(-verbose => 2) },
           'v|verbose!' => \$verbose,
           'quiet'   => sub { $verbose = 0 },
           'debug|d+' => \$debug,
           'output|o=s' => \$output ) or pod2usage(2);
### $debug
### $verbose
### $output
exit 0;
### Pripravuji testovani...
### [<now>] <here> -- Pripravuji testovani...
### $output

for (1..1_000_000) {    ### pracuji [===[%]   ] hotovo
      $output = $output;
}
for (1..15) {      ### prubeh [===[%]    ]
       sleep 2;
}
exit 0;
my($cdb) = new CDB_File($output, "$output.$$") or
        die "$0: new CDB_File failed: $!\n";
while (<>) {
        next if /^$/ or /^#/;
        chop;
        my($k, $v) = split /:/, $_, 2;
        if (defined $v) {
                $cdb->insert($k, $v);
        } else {
                warn "bogus line: $_\n";
        }
}
$cdb->finish or die "$0: CDB_File finish failed: $!\n";

=head1 NAME

live - online monitoring of soccer

=head1 SYNOPSIS

regc [--help] [-o] [-l league] [pattern]

=head1 DESCRIPTION

live is based on Livescore perl module. live acquires data from Livescore and
displays informations about match in text format.

=head1 OPTIONS

=item B<    -v, --verbose>

        verbose output

=item B<    -l, --league> I<region>

        display only matches from selected region.

=item B<    -o <file>, --output <file>>

        Place the output into <file>

=item B<    -h, --help>

        Print this message and exit.

=head1 FILES

temporary files /tmp/livescore_*

=head1 SEE ALSO

Livescore(3pm)

=head1 AUTHOR

Milan Dunghubel

=head1 COPYRIGHT AND LICENSE

This program is free software; you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software Foundation

=cut
