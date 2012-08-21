#
# spec file for package [spectemplate]
#
# Copyright (c) 2010 SUSE LINUX Products GmbH, Nuernberg, Germany.
#
# All modifications and additions to the file contributed by third parties
# remain the property of their copyright owners, unless otherwise agreed
# upon. The license for this file, and modifications and additions to the
# file, is the same license as for the pristine package itself (unless the
# license for the pristine package is not an Open Source License, in which
# case the license is the MIT License). An "Open Source License" is a
# license that conforms to the Open Source Definition (Version 1.9)
# published by the Open Source Initiative.

# Please submit bugfixes or comments via http://bugs.opensuse.org/
#
# norootforbuild

Name:           stromx-studio
Version:
Release:
Summary:        A graphical user interface to edit stromx streams
Group:          Development/Tools/Other

License:        GNU Public License 3
URL:            http://www.stromx.org
Source0:        %{name}-%{version}.tar.bz2

%if 0%{?suse_version}
BuildRequires:  cmake gcc-c++ stromx-devel libqt4-devel
%else 
BuildRequires:  cmake gcc-c++ stromx-devel libqt4-devel lapack
%endif
# Requires:       
# BuildArch:      noarch

%description
This package contains the %{name} application for editing and running stromx streams in a graphical user interface.

%prep
%setup -q


%build
%{__mkdir} build
cd build
cmake -DCMAKE_INSTALL_PREFIX='%{_prefix}' \
      -DCMAKE_BUILD_TYPE=Release \
      -DDOC_INSTALL_DIR=%{_docdir} \
      ..

make %{?_smp_mflags}
make test ARGS="-V"


%install
cd build
make install DESTDIR=%{buildroot}

%clean
rm -rf %{buildroot}

%files
%defattr(0644, root, root, 0755)
%attr(0755, root, root) %{_bindir}/stromx-studio
%defattr(-, root, root, 0755)
%{_docdir}/stromx-studio
%doc


%changelog
