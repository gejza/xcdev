# Before `make install' is performed this script should be runnable with
# `make test'. After `make install' it should work as `perl XC-RD-Make.t'

#########################

# change 'tests => 1' to 'tests => last_test_to_print';

use Test::More tests => 1;
BEGIN { use_ok('XC::RD::Make') };

#########################

# Insert your test code below, the Test::More module is use()ed here so read
# its man page ( perldoc Test::More ) for help writing this test script.

use XC::RD::Make;
my $m = new XC::RD::Make("test.cdb");
$m->insert(1, 'k1', 'k2');
$m->insert(2, 'k1', 'k2');
$m->insert(3, 'k1', 'k2');

