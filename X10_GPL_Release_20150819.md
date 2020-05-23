## building notes

host: Ubuntu 14.04 LTS 32-bit

## dependencies
```
apt-get build-dep linux-lts-xenial
apt install libpcre3-dev unzip

perl -pi -e 's/tool_oldlib/oldlib/' /usr/share/aclocal/libtool.m4 # libxml2 building fails with ranlib error
```

## "X--tag=CC: command not found" error during libxml2 building

```
--- libXML2/Makefile.orig       2020-05-23 17:11:02.334690669 +0200
+++ libXML2/Makefile    2020-05-23 17:11:07.710690669 +0200
@@ -30,6 +30,7 @@
        $(Q)if [ ! -d $(LIBXML2_DIR)/../local ]; then \
        tar zxf libxml2.tar.gz ;\
        cd $(LIBXML2_DIR); ./configure $(XML2_CFG) ; \
+       echo=echo; ECHO=echo; export echo ECHO; \
        make -j; make install; make install-strip;\
        fi
        $(Q)$(CP_CMD) $(LIBXML2_DIR)/../local/lib/*.so $(PRJ_PATH)/lib
```
