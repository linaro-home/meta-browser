#
# This file was derived from the 'Hello World!' example recipe in the
# Yocto Project Development Manual.
#

DESCRIPTION = "Open Content Decryption Module"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://LICENSE;md5=ea83f8bc099c40bde8c4f2441a6eb40b"

SRC_URI = "git://github.com/kuscsik/linaro-cdmi.git;protocol=https;branch=master"
SRCREV_pn-ocdmi ?= "${AUTOREV}"

S = "${WORKDIR}/git"

EXTRA_OECONF_append = "${@base_contains('MACHINE_FEATURES', 'optee', '--enable-aes-ta', '', d)} \
   ${@'--enable-playready' if d.getVar('ENABLE_MS_PLAYREADY', True) == '1' else ''} \
"

# * use-playready : Enables support for Playready CDMI.
#
# * debug-build : Builds OCDM with debug symbols and verbose logging.

DEPENDS_append = " openssl portmap"

DEPENDS_append = "${@base_contains('MACHINE_FEATURES','optee',' optee-aes-decryptor ','',d)}"

# Only ClearKey implementation depends on ssl
DEPENDS_remove = " \
    ${@'openssl' if d.getVar('ENABLE_MS_PLAYREADY', True) == '1' else ''} \
  "

DEPENDS_append = " \
   ${@'playready' if d.getVar('ENABLE_MS_PLAYREADY', True) == '1' else ''} \
   "

do_compile() {
  oe_runmake clean all  -j1
}

inherit autotools
