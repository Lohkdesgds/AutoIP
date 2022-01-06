# AutoIP (multiplatform)
*AutoIP is an app used for Dynv6 ip refreshing if you have a not fixed IP address.*
*You can keep your DNS pointing to you by refreshing each second or so.*
*This app has been tested with IPV6 in mind, but technically it does work with IPV4 too*

### How to use
* *First you have to setup an account on dynv6 website (https://dynv6.com)*
* *Register a URL (probably called Zone or Domain) for your host. Let's say you got `hello.dynv6.net`*
* *Go to its instruction page and get the token (something like `aBCdefghiJKlmnopQRStuvwXyz-012`)*
* *Get this app (build it using cmake on whatever platorm you are or maybe get on releases if thereś one)*
* *Create a batch/shell script (for easier use) with:*
```sh
# On Linux it should look like
./AutoIP -t 120.0 --token `your-fancy-token-here` --hostname `TheURLYouCreated.something.net` -v
# On Windows it should look like
AutoIP.exe -t 120.0 --token `your-fancy-token-here` --hostname `TheURLYouCreated.something.net` -v
```
* *Run the script file*

*It should look like this:*
![Some logging talking about it being set up and its version, then your IP with HTTP 1.1 200 OK](https://media.discordapp.net/attachments/883480599032135690/928703995298611230/SPOILER_unknown.png)

### *Have fun!*