%define name libwpd
%define version 0.5.0
%define  RELEASE 1
%define  release     %{?CUSTOM_RELEASE} %{!?CUSTOM_RELEASE:%RELEASE}

Name: %{name}
Summary: Library for reading and converting WordPerfect(tm) documents.
Version: %{version}
Release: %{release}
Source: %{name}-%{version}.tar.gz
Group: System Environment/Libraries
URL: http://libwpd.sf.net/
BuildRoot: %{_tmppath}/%{name}-%{version}-buildroot
License: LGPL
Prefix: %{prefix}
Requires: glib2 >= 2.0.0
Requires: libgsf >= 1.6.0
BuildRequires: glib2-devel >= 2.0.0
BuildRequires: libgsf-devel >= 1.6.0

%description
Library that handles Word Perfect documents.


%package devel
Requires: libwpd
Summary: Files for developing with libwpd.
Group: Development/Libraries

%description devel
Includes and definitions for developing with libwpd.

%prep
%__rm -rf $RPM_BUILD_ROOT

%setup -q -n %{name}-%{version}

%build
./configure --prefix=%{_prefix} --exec-prefix=%{_prefix} \
	%{?_with_debug:--enable-debug}  \

%__make

%install
umask 022

%__make DESTDIR=$RPM_BUILD_ROOT install

%clean
%__rm -rf $RPM_BUILD_ROOT $RPM_BUILD_DIR/file.list.%{name}

%files
%defattr(755,root,root,755)
%{_bindir}/wpd2html
%{_bindir}/wpd2raw
%{_bindir}/wpd2text
%defattr(644,root,root,755)
%{_libdir}/libwpd-1.a

%files devel
%defattr(644,root,root,755)
%{_libdir}/pkgconfig/libwpd-1.pc
%{_includedir}/libwpd-1/libwpd

%changelog
* Sat Apr 26 2003 Rui M. Seabra <rms@1407.org>
- Create rpm spec
