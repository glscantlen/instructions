################################################################################
#
# xdriver_xf86-video-armsoc
#
################################################################################

XDRIVER_XF86_VIDEO_ARMSOC_VERSION = 13a46a857369cf3627cf6d4d6f04e5420341cde3
XDRIVER_XF86_VIDEO_ARMSOC_SITE = $(call github,mihailescu2m,xf86-video-armsoc,$(XDRIVER_XF86_VIDEO_ARMSOC_VERSION))
#XDRIVER_XF86_VIDEO_ARMSOC_VERSION = 1.0.0
#XDRIVER_XF86_VIDEO_ARMSOC_SITE = $(call github,endlessm,xf86-video-armsoc,$(XDRIVER_XF86_VIDEO_ARMSOC_VERSION))
XDRIVER_XF86_VIDEO_ARMSOC_AUTORECONF = YES
XDRIVER_XF86_VIDEO_ARMSOC_DEPENDENCIES = xserver_xorg-server xproto_fontsproto xproto_randrproto xproto_renderproto xproto_xextproto xproto_xproto
XDRIVER_XF86_VIDEO_ARMSOC_CONF_OPTS = --with-drmmode=exynos --enable-maintainer-mode
XDRIVER_XF86_VIDEO_ARMSOC_PRE_CONFIGURE_HOOKS += M4MKDIR_HOOK

define M4MKDIR_HOOK
-mkdir $(@D)/m4;
endef

define XDRIVER_XF86_VIDEO_ARMSOC_AUTOGEN_HOOK
( \
cd $(@D); \
mkdir -p config; \
touch AUTHORS; \
$(HOST_DIR)/usr/bin/aclocal; \
$(HOST_DIR)/usr/bin/autoheader; \
$(HOST_DIR)/usr/bin/libtoolize --copy --force; \
$(HOST_DIR)/usr/bin/automake --copy --add-missing --foreign; \
$(HOST_DIR)/usr/bin/autoconf; \
);
endef

XDRIVER_XF86_VIDEO_ARMSOC_PRE_CONFIGURE_HOOKS += XDRIVER_XF86_VIDEO_ARMSOC_AUTOGEN_HOOK

$(eval $(autotools-package))
