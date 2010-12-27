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

use Test::More tests => 4;
use Math::Complex;

$z1 = Math::Complex->make(1, 2);
$z2 = Math::Complex->make(3, 4);
$z3 = Math::Complex->make(4, 6);
$z4 = Math::Complex->make(-8, 10);

ok(defined $z1, "metoda make chodi. Vytvoreno: '$z1'");
ok($z1->isa("Math::Complex"), "metoda make vraci objekt typu Math::Complex");
is($z1 + $z2, $z3, "scitani funguje");
is($z1 * $z2, $z4, "nasobeni funguje");
#done_testing(2);


