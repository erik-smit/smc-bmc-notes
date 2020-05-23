## building notes

host: Ubuntu 14.04 LTS 32-bit

## building
```
make menuconfig # disable stuff you don't need
make x10 ver=2.15
```

## dependencies
```
apt-get build-dep linux-lts-xenial
apt install libpcre3-dev unzip

perl -pi -e 's/tool_oldlib/oldlib/' /usr/share/aclocal/libtool.m4 # libxml2 building fails with ranlib error
```

### jarsigner: unable to sign jar: java.net.UnknownHostException: timestamp.geotrust.com

```
--- JavaAP/Makefile.orig        2020-05-23 17:48:34.486690669 +0200
+++ JavaAP/Makefile     2020-05-23 17:48:41.718690669 +0200
@@ -14,7 +14,7 @@
 MANIFEST                  = $(JAVAAP_DIR)/MYMANIFEST.MF
 STOREPASS          = SuperSMC4Cert
 KEYPASS            = SuperSMC4Cert
-TIME_SERVER               = https://timestamp.geotrust.com/tsa
+TIME_SERVER               = http://tsa.starfieldtech.com
 PROVIDER           = smccert
 ANT_ZIP            = apache-ant-1.7.1-bin.tar.gz
 ANT_HOME           = $(JAVAAP_DIR)/apache-ant-1.7.1
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
