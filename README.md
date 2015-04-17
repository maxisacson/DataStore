# DataStore

### Compile `example.cxx`
```
$ g++ -std=c++11 -Wall -O2 -I.. example.cxx -oexample.exe
```

### Compile `rootexample.cxx`
```
$ g++ -std=c++11 -g -O2 -pipe -Wall -W -Woverloaded-virtual -fPIC -Iinclude  -pthread -I $ROOTSYS/include -o rootexample.o -c rootexample.cxx
$ g++ -std=c++11 -g -O2 -m64 rootexample.o $(root-config --libs) -o rootexample.exe
