OCDM_GIT_BRANCH="master"
OCDM_DESTSUFIX="ocdm"

SRC_URI += "${@bb.utils.contains('PACKAGECONFIG', 'use-ocdm', '\
      git://github.com/linaro-home/open-content-decryption-module.git;protocol=https;branch=${OCDM_GIT_BRANCH};name=ocdm;destsuffix=${OCDM_DESTSUFIX}\
    ', '', d)}"
SRCREV_ocdm = "${AUTOREV}"
DEPENDS_append = " ${@bb.utils.contains('PACKAGECONFIG', 'use-ocdm', 'ocdmi', '', d)} "

python add_ocdm_patches() {
    import glob
    srcdir = d.getVar('WORKDIR', True)
    d.appendVar('SRC_URI', "file://" + srcdir + "/ocdm/patch/add_ocdm_keyssystems.patch")
}

copy_ocdm_files() {
    cp -r ${WORKDIR}/ocdm ${S}/media/cdm/ppapi/external_open_cdm
}

do_patch[prefuncs] += "${@bb.utils.contains('PACKAGECONFIG', 'use-ocdm', 'add_ocdm_patches', '', d)}"
do_unpack[postfuncs] += "${@bb.utils.contains('PACKAGECONFIG', 'use-ocdm', 'copy_ocdm_files', '', d)}"

do_compile_append() {
    if [ -n "${@bb.utils.contains('PACKAGECONFIG', 'use-ocdm', 'use-ocdm', '', d)}" ]; then
        ninja -C ${S}/out/${CHROMIUM_BUILD_TYPE} opencdmadapter
    fi
}

do_install_append() {
    if [ -n "${@bb.utils.contains('PACKAGECONFIG', 'use-ocdm', 'use-ocdm', '', d)}" ]; then
        install -Dm 0755 ${B}/out/${CHROMIUM_BUILD_TYPE}/libopencdmadapter.so \
            ${D}${libdir}/${BPN}/libopencdmadapter.so
        install -Dm 0755 ${B}/out/${CHROMIUM_BUILD_TYPE}/libopencdm.so \
            ${D}${libdir}/${BPN}/libopencdm.so
    fi
}
