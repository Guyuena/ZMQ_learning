Name:          zeromq
Version:       4.1.8
Release:       1%{?dist}
Summary:       The ZeroMQ messaging library
Group:         Applications/Internet
License:       LGPLv3+
URL:           http://www.zeromq.org/
Source:        http://download.zeromq.org/%{name}-%{version}.tar.gz
Prefix:        %{_prefix}
Buildroot:     %{_tmppath}/%{name}-%{version}-%{release}-root
BuildRequires: gcc, make, gcc-c++, libstdc++-devel, asciidoc, xmlto
Requires:      libstdc++

#
# Conditional build options
# Default values are:
#    --without-libgssapi_krb5
#    --without-libsodium
#    --without-pgm
#

# If neither macro exists, use the default value.
%{!?_with_libgssapi_krb5: %{!?_without_libgssapi_krb5: %define _without_libgssapi_krb5 --without-liblibgssapi_krb5}}
%{!?_with_libsodium: %{!?_without_libsodium: %define _without_libsodium --without-libsodium}}
%{!?_with_pgm: %{!?_without_pgm: %define _without_pgm --without-pgm}}

# It's an error if both --with and --without options are specified
%{?_with_libgssapi_krb5: %{?_without_libgssapi_krb5: %{error: both _with_libgssapi_krb5 and _without_libgssapi_krb5}}}
%{?_with_libsodium: %{?_without_libsodium: %{error: both _with_libsodium and _without_libsodium}}}
%{?_with_pgm: %{?_without_pgm: %{error: both _with_pgm and _without_pgm}}}

%{?_with_libgssapi_krb5:BuildRequires: krb5-devel}
%{?_with_libgssapi_krb5:Requires: krb5-libs}

%{?_with_libsodium:BuildRequires: libsodium-devel}
%{?_with_libsodium:Requires: libsodium}

%{?_with_pgm:BuildRequires: openpgm-devel}
%{?_with_pgm:Requires: openpgm}

%ifarch pentium3 pentium4 athlon i386 i486 i586 i686 x86_64
%{!?_with_pic: %{!?_without_pic: %define _with_pic --with-pic}}
%{!?_with_gnu_ld: %{!?_without_gnu_ld: %define _with_gnu_ld --with-gnu_ld}}
%endif

%description
The 0MQ lightweight messaging kernel is a library which extends the
standard socket interfaces with features traditionally provided by
specialised messaging middleware products. 0MQ sockets provide an
abstraction of asynchronous message queues, multiple messaging
patterns, message filtering (subscriptions), seamless access to
multiple transport protocols and more.

This package contains the ZeroMQ shared library.

%package devel
Summary:  Development files and static library for the ZeroMQ library
Group:    Development/Libraries
Requires: %{name} = %{version}-%{release}, pkgconfig

%description devel
The 0MQ lightweight messaging kernel is a library which extends the
standard socket interfaces with features traditionally provided by
specialised messaging middleware products. 0MQ sockets provide an
abstraction of asynchronous message queues, multiple messaging
patterns, message filtering (subscriptions), seamless access to
multiple transport protocols and more.

This package contains ZeroMQ related development libraries and header files.

%prep
%setup -q

%build
%configure \
    %{?_with_libsodium} \
    %{?_without_libsodium} \
    %{?_with_pgm} \
    %{?_without_pgm} \
    %{?_with_libgssapi_krb5} \
    %{?_without_libgssapi_krb5} \
    %{?_with_pic} \
    %{?_without_pic} \
    %{?_with_gnu_ld} \
    %{?_without_gnu_ld}

%{__make} %{?_smp_mflags}

%install
[ "%{buildroot}" != "/" ] && %{__rm} -rf %{buildroot}

# Install the package to build area
%{__make} check
%makeinstall

%post
/sbin/ldconfig

%postun
/sbin/ldconfig

%clean
[ "%{buildroot}" != "/" ] && %{__rm} -rf %{buildroot}

%files
%defattr(-,root,root,-)

# docs in the main package
%doc AUTHORS ChangeLog COPYING COPYING.LESSER NEWS

# binaries
%{_bindir}/curve_keygen

# libraries
%{_libdir}/libzmq.so.*

%{_mandir}/man7/zmq.7.gz

%files devel
%defattr(-,root,root,-)
%{_includedir}/zmq.h
%{_includedir}/zmq_utils.h

%{_libdir}/libzmq.la
%{_libdir}/libzmq.a
%{_libdir}/pkgconfig/libzmq.pc
%{_libdir}/libzmq.so

%{_mandir}/man3/zmq_bind.3.gz
%{_mandir}/man3/zmq_close.3.gz
%{_mandir}/man3/zmq_connect.3.gz
%{_mandir}/man3/zmq_disconnect.3.gz
%{_mandir}/man3/zmq_ctx_get.3.gz
%{_mandir}/man3/zmq_ctx_new.3.gz
%{_mandir}/man3/zmq_ctx_set.3.gz
%{_mandir}/man3/zmq_ctx_term.3.gz
%{_mandir}/man3/zmq_msg_recv.3.gz
%{_mandir}/man3/zmq_errno.3.gz
%{_mandir}/man3/zmq_getsockopt.3.gz
%{_mandir}/man3/zmq_msg_close.3.gz
%{_mandir}/man3/zmq_msg_copy.3.gz
%{_mandir}/man3/zmq_msg_data.3.gz
%{_mandir}/man3/zmq_msg_init.3.gz
%{_mandir}/man3/zmq_msg_init_data.3.gz
%{_mandir}/man3/zmq_msg_init_size.3.gz
%{_mandir}/man3/zmq_msg_move.3.gz
%{_mandir}/man3/zmq_msg_size.3.gz
%{_mandir}/man3/zmq_msg_get.3.gz
%{_mandir}/man3/zmq_msg_more.3.gz
%{_mandir}/man3/zmq_msg_send.3.gz
%{_mandir}/man3/zmq_msg_set.3.gz
%{_mandir}/man3/zmq_poll.3.gz
%{_mandir}/man3/zmq_proxy.3.gz
%{_mandir}/man3/zmq_recv.3.gz
%{_mandir}/man3/zmq_recvmsg.3.gz
%{_mandir}/man3/zmq_send.3.gz
%{_mandir}/man3/zmq_sendmsg.3.gz
%{_mandir}/man3/zmq_setsockopt.3.gz
%{_mandir}/man3/zmq_socket.3.gz
%{_mandir}/man3/zmq_socket_monitor.3.gz
%{_mandir}/man3/zmq_strerror.3.gz
%{_mandir}/man3/zmq_version.3.gz
%{_mandir}/man3/zmq_unbind.3.gz
%{_mandir}/man3/zmq_ctx_shutdown.3.gz
%{_mandir}/man3/zmq_has.3.gz
%{_mandir}/man3/zmq_msg_gets.3.gz
%{_mandir}/man3/zmq_proxy_steerable.3.gz
%{_mandir}/man7/zmq_inproc.7.gz
%{_mandir}/man7/zmq_ipc.7.gz
%{_mandir}/man7/zmq_pgm.7.gz
%{_mandir}/man7/zmq_tcp.7.gz
%{_mandir}/man3/zmq_curve_keypair.3.gz
%{_mandir}/man3/zmq_send_const.3.gz
%{_mandir}/man3/zmq_z85_decode.3.gz
%{_mandir}/man3/zmq_z85_encode.3.gz
%{_mandir}/man7/zmq_curve.7.gz
%{_mandir}/man7/zmq_null.7.gz
%{_mandir}/man7/zmq_plain.7.gz
%{_mandir}/man7/zmq_tipc.7.gz

%changelog
* Sat Oct 25 2014 Phillip Mienk <mienkphi@gmail.com>
- Add --with/--without libgssapi_krb5 support following J.T.Conklin's pattern

* Sat Oct 18 2014 J.T. Conklin <jtc@acorntoolworks.com>
- Add --with/--without pgm support
- Add --with/--without libsodium support

* Tue Jun 10 2014 Tristian Celestin <tristian.celestin@outlook.com> 4.0.4
- Updated packaged files

* Mon Nov 26 2012 Justin Cook <jhcook@gmail.com> 3.2.2
- Update packaged files

* Fri Apr 8 2011 Mikko Koppanen <mikko@kuut.io> 3.0.0-1
- Update dependencies and packaged files

* Sat Apr 10 2010 Mikko Koppanen <mkoppanen@php.net> 2.0.7-1
- Initial packaging
