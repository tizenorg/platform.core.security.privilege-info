Name:    privilege-info
Summary: Privilege Management
Version: 0.0.1
Release: 1
Group:   System/Libraries
License: Apache-2.0
Source0: %{name}-%{version}.tar.gz

Requires(post):   /sbin/ldconfig
Requires(postun): /sbin/ldconfig
BuildRequires: cmake
BuildRequires: pkgconfig(dlog)
BuildRequires: pkgconfig(capi-base-common)
BuildRequires: pkgconfig(capi-security-privilege-manager)

%description
Privilege Information

%package -n privilege-info-devel
Summary: Privilege Info API (Development)
Group: TO_BE/FILLED_IN
Requires: %{name} = %{version}-%{release}

%description -n privilege-info-devel
The Privilege Info API provides functions to get privilege information (DEV)

%package -n tc-privilege-info
Summary: tc-privilege-info
Group: TO_BE/FILLED_IN
Requires: %{name} = %{version}-%{release}

%description -n tc-privilege-info
tc-privilege-info

%prep
%setup -q

%build

%if "%{?tizen_profile_name}" == "wearable"
    __PROFILE_TYPE="WEARABLE"
%else
    __PROFILE_TYPE="MOBILE"
%endif

export CFLAGS="$CFLAGS -DTIZEN_ENGINEER_MODE"
export CXXFLAGS="$CXXFLAGS -DTIZEN_ENGINEER_MODE"
export FFLAGS="$FFLAGS -DTIZEN_ENGINEER_MODE"

echo cmake . -DPREFIX=%{_prefix} \
        -DEXEC_PREFIX=%{_exec_prefix} \
        -DLIBDIR=%{_libdir} \
        -DINCLUDEDIR=%{_includedir} \
        -DCMAKE_BUILD_TYPE=%{build_type} \
        -DVERSION=%{version} \
        -DDPL_LOG="ON" \
        -DDATADIR=%{_datadir} \
        -DPROFILE_TYPE="${__PROFILE_TYPE}"

cmake . -DPREFIX=%{_prefix} \
        -DEXEC_PREFIX=%{_exec_prefix} \
        -DLIBDIR=%{_libdir} \
        -DINCLUDEDIR=%{_includedir} \
        -DCMAKE_BUILD_TYPE=%{build_type} \
        -DVERSION=%{version} \
        -DDPL_LOG="ON" \
        -DDATADIR=%{_datadir} \
        -DPROFILE_TYPE="${__PROFILE_TYPE}"

make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
mkdir -p %{buildroot}%{_datadir}/license
cp LICENSE.Apache-2.0 %{buildroot}%{_datadir}/license/privilege-info

%make_install

%files -n privilege-info
%{_libdir}/libprivilege-info.so.*
%{_datadir}/license/privilege-info

%files -n privilege-info-devel
%{_libdir}/libprivilege-info.so
%{_includedir}/*.h
%{_libdir}/pkgconfig/privilege-info.pc

%files -n tc-privilege-info
%{_bindir}/tc-privilege-info

%clean
rm -rf %{buildroot}

