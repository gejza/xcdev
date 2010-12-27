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
use Getopt::Long qw(Configure GetOptions);
Configure("bundling");

use Smart::Comments;

my($output) = 'a.cdb';

GetOptions('output=s' => \$output, 'o=s' => \$output);

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

