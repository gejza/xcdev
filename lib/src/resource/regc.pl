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

# libapache-configfile-perl
#use strict;
use warnings;

use Getopt::Long;
use Pod::Usage;


my $output = 'a.cdb';
my $verbose = 1;
my $debug = 0;

## Parse options and print usage if there is a syntax error,
## or if usage was explicitly requested.
GetOptions('help|h|?' => sub { pod2usage(-verbose => 1) },
           man => sub { pod2usage(-verbose => 2) },
           'v|verbose+' => \$verbose,
           'quiet'   => sub { $verbose = 0 },
           'debug|d+' => \$debug,
           'output|o=s' => \$output ) or pod2usage(2);
if ($debug > 0){
    use Smart::Comments "###", "####";
}
### $debug
### $verbose
### $output
#for (@ARGV){
#    print $_."\n";
#}

  use Apache::ConfigParser;

  # Create a new empty parser.
  my $c1 = Apache::ConfigParser->new;

  # Load an Apache configuration file.
  my $rc = $c1->parse_file('test.conf');

  # If there is an error in parsing the configuration file, then $rc
  # will be false and an error string will be available.
  if (not $rc) {
    die $c1->errstr, "\n";
  }

  # Get the root of a tree that represents the configuration file.
  # This is an Apache::ConfigParser::Directive object.
  my $root = $c1->root;

  ### $rc
  use Data::Dumper;
  print Data::Dumper->Dump($rc);
  # Get all of the directives and starting of context's.
  my @directives = $root->daughters;

  # Get the first directive's name.
  my $d_name = $directives[0]->name;

  # This directive appeared in this file, which may be in an Include'd
  # file.
  my $d_filename = $directives[0]->filename;

  # And it begins on this line number.
  my $d_line_number = $directives[0]->line_number;

  # Find all the CustomLog entries, regardless of context.
  my @custom_logs = $c1->find_down_directive_names('CustomLog');

  # Get the first CustomLog.
  my $custom_log = $custom_logs[0];

  # Get the value in string form.
  $custom_log_args = $custom_log->value;

  # Get the value in array form already split.
  my @custom_log_args = $custom_log->get_value_array;

  # Get the same array but a reference to it.
  my $customer_log_args = $custom_log->value_array_ref;

  # The first value in a CustomLog is the filename of the log.
  my $custom_log_file = $custom_log_args->[0];

  # Get the original value before the path has been made absolute.
  @custom_log_args   = $custom_log->get_orig_value_array;
  $customer_log_file = $custom_log_args[0];

  # Here is a more complete example to load an httpd.conf file and add
  # a new VirtualHost directive to it.
  #
  # The Apache::ConfigParser object contains a reference to a
  # Apache::ConfigParser::Directive object, which can be obtained by
  # using Apache::ConfigParser->root.  The root node is a
  # Apache::ConfigParser::Directive which ISA Tree::DAG_Node (that is
  # Apache::ConfigParser::Directive's @ISA contains Tree::DAG_Node).
  # So to get the root node and add a new directive to it, it could be
  # done like this:
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
#use CDB_File;
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
