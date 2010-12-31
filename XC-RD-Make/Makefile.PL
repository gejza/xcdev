use 5.010001;
use ExtUtils::MakeMaker;

$CC = 'g++';

# See lib/ExtUtils/MakeMaker.pm for details of how to influence
# the contents of the Makefile that is written.
WriteMakefile(
    NAME              => 'XC::RD::Make',
    VERSION_FROM      => 'lib/XC/RD/Make.pm', # finds $VERSION
    PREREQ_PM         => {}, # e.g., Module::Name => 1.1
    ($] >= 5.005 ?     ## Add these new keywords supported since 5.005
      (ABSTRACT_FROM  => 'lib/XC/RD/Make.pm', # retrieve abstract from module
       AUTHOR         => 'Milan Dunghubel <gejza@>') : ()),
    LIBS              => ['-lxcrdmake'], # e.g., '-lm'
    DEFINE            => '', # e.g., '-DHAVE_SOMETHING'
	CC                => $CC,
	LD          	  => '$(CC)',
    INC               => '-I.', # e.g., '-I. -I/usr/include/other'
	# Un-comment this if you add C files to link with later:
    # OBJECT            => '$(O_FILES)', # link all the C files too
    XSOPT             => '-C++',
	TYPEMAPS          => ['perlobject.map' ],
);
