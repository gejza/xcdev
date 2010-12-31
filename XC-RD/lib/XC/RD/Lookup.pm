package XC::RD::Lookup;

use 5.010001;
use strict;
use warnings;

require Exporter;

our @ISA = qw(Exporter);

# Items to export into callers namespace by default. Note: do not export
# names by default without a very good reason. Use EXPORT_OK instead.
# Do not simply export all your public functions/methods/constants.

# This allows declaration	use XC::RD::Lookup ':all';
# If you do not need this, moving things directly into @EXPORT or @EXPORT_OK
# will save memory.
our %EXPORT_TAGS = ( 'all' => [ qw(
	
) ] );

our @EXPORT_OK = ( @{ $EXPORT_TAGS{'all'} } );

our @EXPORT = qw(
	
);

our $VERSION = '0.01';

require XSLoader;
XSLoader::load('XC::RD::Lookup', $VERSION);

# Preloaded methods go here.

1;
__END__
# Below is stub documentation for your module. You'd better edit it!

=head1 NAME

XC::RD::Lookup - Perl extension for blah blah blah

=head1 SYNOPSIS

  use XC::RD::Lookup;
  my $m = new XC::RD::Lookup("test.cdb");
  $m->insert(1, 'key', 'value');

=head1 DESCRIPTION

Stub documentation for XC::RD::Lookup, created by h2xs. It looks like the
author of the extension was negligent enough to leave the stub
unedited.

=head2 Methods

=over 4

=item * $object->verbose(true or false)

=item * $object->verbose()

Returns the value of the 'verbose' property.  When called with an
argument, it also sets the value of the property.  Use a true or false
Perl value, such as 1 or 0.

=item * $object->hoot()

Returns a hoot if we're supposed to be verbose.  Otherwise it returns
nothing.

=back

=head2 EXPORT

None by default.

=head1 SEE ALSO

Mention other useful documentation such as the documentation of
related modules or operating system documentation (such as man pages
in UNIX), or any relevant external documentation such as RFCs or
standards.

If you have a mailing list set up for your module, mention it here.

If you have a web site set up for your module, mention it here.

=head1 AUTHOR

Milan Dunghubel, E<lt>gejza@E<gt>

=head1 COPYRIGHT AND LICENSE

Copyright (C) 2010 by Milan Dunghubel

This library is free software; you can redistribute it and/or modify
it under the same terms as Perl itself, either Perl version 5.10.1 or,
at your option, any later version of Perl 5 you may have available.


=cut
