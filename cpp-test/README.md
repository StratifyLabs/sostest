# HelloWorld

This project shows just how easy it is to write programs for Stratify OS.  Stratity OS takes care of all the hardware initialization (including stdio initialization) so you can just start writing code.

If you haven't already done so, you need to get set up with some hardware and the software tools using the [Stratify Labs web application](https://app.stratifylabs.co/) which has installation instructions and tutorials.

Once you install the command line tool, you can clone and build HelloWorld using:

```
sl application.create:name=HelloWorld
sl application.build:path=HelloWorld
sl application.install:path=HelloWorld,run,terminal # need connected hardware for this one
```
