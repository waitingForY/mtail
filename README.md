# Mtail  
## introduction  
This program is a free software.its a tool like the tail command in unix(linux),which  
mtail does the equivalent of tail -f on several files at once.The screen is split into  
as many parts as there are files to watch.And there is one option you can choose to show  
which file display.its -h.
## dependence  
A Unix-like operating system and a C compiler are required. Known to work on AIX, Linux, and others.   
When you use it on linux you should choose tail_linux version,while aix and others system choose tail_aix version.  

## Install

```
git clone --recursive https://github.com/waitingForY/mtail.git
cd mtail
$ make
# make install    		# To install in /usr/local .it's not necessary
# make PREFIX=/some/path install	# To install in /some/path .it's not necessary
```
## Usage
```
cd mtail
./tail -n somepath/ 
or you can use * in the path to choose which file shou be tail,like flow
./mtail -n somepath/test* 
if use it without option ,it will display without and head information;
./mtail somepath/
```

