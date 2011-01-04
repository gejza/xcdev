# Before `make install' is performed this script should be runnable with
# `make test'. After `make install' it should work as `perl XC-RD-Make.t'

#########################

# change 'tests => 1' to 'tests => last_test_to_print';
use strict;

use Test::More tests => 1;
BEGIN { use_ok('XC::RD') };

#########################

# Insert your test code below, the Test::More module is use()ed here so read
# its man page ( perldoc Test::More ) for help writing this test script.

use XC::RD;
my $m = new XC::RD::Make("test.cdb");

my %skoly = ("pavel" => "FI MUNI", "petr" => "FIT VUT", "pole" => [ 1,2,3]);
$m->insert(1, "skoly", \%skoly);
