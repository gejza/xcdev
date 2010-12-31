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
my $ns;
my $key;
for ($ns = 1; $ns < 50; $ns++) {
	for ($key = 1; $key < 50000; $key++) {
		$m->insert($ns, "$key", "$ns:$key");
	}
}

