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
```
until finished
```

End with an example of getting some data out of the system or using it for a little demo

## Running the tests

Explain how to run the automated tests for this system

### Break down into end to end tests

Explain what these tests test and why

```
Give an example
```

### And coding style tests

Explain what these tests test and why

```
Give an example
```

## Deployment

Add additional notes about how to deploy this on a live system

## Built With

* [Dropwizard](http://www.dropwizard.io/1.0.2/docs/) - The web framework used
* [Maven](https://maven.apache.org/) - Dependency Management
* [ROME](https://rometools.github.io/rome/) - Used to generate RSS Feeds

## Contributing

Please read [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) for details on our code of conduct, and the process for submitting pull requests to us.

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/your/project/tags). 

## Authors

* **Billie Thompson** - *Initial work* - [PurpleBooth](https://github.com/PurpleBooth)

See also the list of [contributors](https://github.com/your/project/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Hat tip to anyone whose code was used
* Inspiration
* etc
