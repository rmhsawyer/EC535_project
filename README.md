# EC535 Project Bluetooth Music Player With LCD Control And LED Display

As there are already many smart home music player products in the market, we plan to make one ourselves with some additional interesting features. This product is mainly comprised of four features: LCD control and display panel, Bluetooth MP3 player, IR sensors to detect movements, and multiple LED-strip lighting modes. The device can get the number of people in the room and automatically switch on/off the music as well as lighting according to the presence of the people. The user is also able to control the lighting effect, choose music, change the number of people via a touch screen. The actual product achieves all the planned features and works robustly.

## Getting Started

To get our project running on your gumstix board. You may first get your bluetooth device being able to be connected with your gumstix. Following setup steps at the following link:
https://wiki.gumstix.com/index.php/Category:How_to_-_bluetooth
```
“hcitool dev”: returns your device’s own address.
“hcitool scan”: lists all nearby Bluetooth devices.
“sdptool browse” returns more in-depth information.
```

### Prerequisites
Download the music_control.sh into the directory /media/card/project/scripts/

### Installing

For the Qt app, you can go to the github directory,then go to /projectqt/ folder. Type in terminal

```
make
```
A qt executable app whose name is "qt" will be generated. By sending this app to gumstix, you can have the app running on your device.

For the kernel module, you can go to /km/ folder, and also type in terminal
```
make
```
A .ko file whose name is mygpio.ko will be generated. You may need to send this kernel module to your gumstix.


Also remember to send the "build.sh" shell file to your gumstix. It includes all commands to insert the kernel module, set all export variables, and start the qt application.

## Built With

* [C++]
* [C]
* [Shell]
* [Qt]
* [Madplay]
* [Aplay]

## Contributing

Please read [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) for details on our code of conduct, and the process for submitting pull requests to us.

## Youtube Links
Some test video have been uploaded to Youtube. You can watch them by clicking the following links.\\
https://www.youtube.com/watch?v=cWzPH9PpAWI

https://www.youtube.com/watch?v=_dV1vEtFRS8

https://www.youtube.com/watch?v=uD4pkmpM5RQ

## Authors

* **Zhiyu Wang**
* **Minghe Ren**
* **Zhiwei Tang**
