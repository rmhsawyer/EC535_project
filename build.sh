mknod /dev/mygpio c 61 0
insmod mykernel.ko

export QWS_MOUSE_PROTO='tslib:/dev/input/touchscreen0'
export TSLIB_CONFFILE=/etc/ts.conf
export TSLIB_PLUGINDIR=/usr/lib
export TSLIB_TSDEVICE=/dev/input/event0
export TSLIB_FBDEVICE=/dev/fb0
export TSLIB_CONSOLEDEVICE=/dev/tty
export QT_QWS_FONTDIR=/media/card/lib/fonts
export TSLIB_PLUGINDIR=/usr/lib/ts
./qt -qws
