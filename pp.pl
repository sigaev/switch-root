#!/usr/bin/perl -wI/home/sigaev/perl5/lib
# perl5 "Tue Mar 22 05:32:41 2005" Dmitry Sigaev http://sigaev.net
# 1-10 Sep 2004, 20-22 Mar 2005
# Perl Preprocessor v1.0 with XML support

use strict;
use Getopt::Std;

binmode STDOUT;
my %opt;
getopts('c:', \%opt);
my ($var, $script, $xml) = ('', $0 =~ /^.*?([^\/\\]*)$/);
$xml = 'x' eq lc substr($script, 0, 1);
die "usage: $script [-c @{[$xml ? 'XML-' : '']}CONFIG-FILE] [SOURCE-FILE]\n"
	unless $opt{c} ? 1 > $#ARGV : 0 == $#ARGV;

if($opt{c})
{
	my %val = ();
	if($xml)
	{
		require XML::Simple;
		# load XML into a hashref
		my $ref = XML::Simple::XMLin($opt{c},
			searchpath => [ '.' ], noattr => 1, suppressempty => 1);

		# transform loaded values into a simple hash %val
		foreach my $a (keys %$ref)
		{
			foreach(keys %{$ref->{$a}})
			{
				die "error: Name conflict in $opt{c} ($_)\n"
					if defined $val{$_};
				die "error: Complex variable in $opt{c} ($_)\n"
					if ref($val{$_} = $ref->{$a}->{$_});
			}
		}
	}
	else
	{
		local $_;
		# load CONFIG into $_
		open F, $opt{c} or die "fatal/open($opt{c}): $!\n";
		{
			local $/;
			$_ = <F>;
		}
		close F or die "fatal/close($opt{c}): $!\n";

		# transform loaded values into a simple hash %val
		foreach(/\(([^\(\)]+)\)/g)
		{
			if(/^\s*(\S+)\s+(.*?)\s*$/s)
			{
				local $_ = $1;
				# ^^^ to make the rest look like the above
				die "error: Name conflict in $opt{c} ($_)\n"
					if defined $val{$_};
				$val{$_} = $2;
			}
		}
	}

# properly arrange and "print" that simple hash
my %out = ();
while(scalar keys %val)
{
	my $err = 1;
	# ^^^ to die if nothing is printed during a pass
	foreach(keys %val)
	{
		my $ok = 1;
		# ^^^ is it ok to print this key?
		foreach my $a ($val{$_} =~ /\$\{?(\w+)/g)
		{	# check all references; do NOT try $_ here!
			$ok = 0 unless grep /^$a$/, keys %out, 'ENV';
			# ^^^ don't print the key before its references
		}
		if($ok)
		{
			$val{$_} =~ s/([\\"])/\\$1/g;
			$out{$_} = $val{$_};
			$var .= "my \$$_ = \"$val{$_}\";\n";
			delete $val{$_};
			$err = 0;
		}
	}
	local $" = ', ';
	die "error: Undefined references in $opt{c} (@{[keys %val]})\n" if $err;
}

	my $pwd = '';
	$opt{c} =~ s/\\/\//g;
	if('/' ne substr($opt{c}, 0, 1))
	{
		local $_ = `pwd`; chomp; s/\\/\//g; $pwd = "$_/";
		die "error: Cannot find pwd\n" unless $_;
	}
	$var .= "my \$Config = \"$pwd$opt{c}\";\n";

	eval $var;
	die $@ if $@;

	if($#ARGV)
	{
		print $var;
		exit 0;
	}
}

open F, $ARGV[0] or die "fatal/open($ARGV[0]): $!\n";
{
	local $/;
	$_ = "\n".<F>;
}
close F or die "fatal/close($ARGV[0]): $!\n";

s/\n\$\$[ \t]+(.+)/\n\$\$beg\n$1\n\$\$end/g;
my ($out, $pos) = ('', 0);
while(1)
{
	my $beg = index $_, "\n\$\$beg", $pos;
	my $end = index $_, "\n\$\$end", $pos;
	last if -1 == $beg and -1 == $end;
	die "error($ARGV[0]): \$\$beg/\$\$end mismatch\n"
		if	-1 == $beg or
			$beg > $end or
			-1 != index substr($_, $beg, $end - $beg), "\n\$\$beg", 1;

	my $tmp = substr $_, $pos, $beg - $pos;
	$tmp =~ s/([\\\%])/\\$1/g;
	$out .= $tmp;
	$tmp = substr $_, $beg, $end - $beg;
	$tmp =~ s/\n.+/\nEOF/;
	$out .= $tmp;
	$pos = index $_, "\n", $end + 1;
	$pos = length $_ if -1 == $pos;
	$tmp = substr $_, $end, $pos - $end;
	$tmp =~ s/\n.+/\nprint <<EOF;/;
	$out .= $tmp;
}
substr($_, $pos) =~ s/([\\\%])/\\$1/g;
$out .= substr $_, $pos;
chomp $out;
eval "$var\nprint <<EOF;$out\nEOF\n";
die $@ if $@;
exit 0;
