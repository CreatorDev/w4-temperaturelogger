include $(TOPDIR)/rules.mk

# Name and release number of this package
PKG_NAME:=temperaturelogger
PKG_VERSION:=1.0.0
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/temperaturelogger
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Ci40-temperature-logger awa
  DEPENDS:=+awalwm2m +letmecreate
endef

define Package/temperaturelogger/description
	Awa Ci40-temperature-logger example
endef

TARGET_CFLAGS += -I$(STAGING_DIR)/usr/include
TARGET_LDFLAGS += -L$(STAGING_DIR)/usr/lib

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) TemperatureLogger/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
	$(TARGET_CONFIGURE_OPTS) \
	CFLAGS="$(TARGET_CFLAGS)" \
	LDFLAGS="$(TARGET_LDFLAGS)"
endef

define Package/temperaturelogger/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/temperaturelogger $(1)/bin/
endef

$(eval $(call BuildPackage,temperaturelogger))
