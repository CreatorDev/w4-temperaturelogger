include $(TOPDIR)/rules.mk

# Name and release number of this package
PKG_NAME:=ci40-temperature-logger
PKG_VERSION:=1.0.0
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/ci40-temperature-logger
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Ci40-temperature-logger awa
  DEPENDS:=+awalwm2m +letmecreate
endef

define Package/ci40-temperature-logger/description
	Awa Ci40-temperature-logger example
endef

TARGET_CFLAGS += -I$(STAGING_DIR)/usr/include
TARGET_LDFLAGS += -L$(STAGING_DIR)/usr/lib

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) Ci40-temperature-logger/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
	$(TARGET_CONFIGURE_OPTS) \
	CFLAGS="$(TARGET_CFLAGS)" \
	LDFLAGS="$(TARGET_LDFLAGS)"
endef

define Package/ci40-temperature-logger/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ci40-temperature-logger $(1)/bin/
endef

$(eval $(call BuildPackage,ci40-temperature-logger))
