# Ucode-Connect Track C uchat - MonkeyChat!

---

![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![JSON](https://img.shields.io/badge/json-5E5C5C?style=for-the-badge&logo=json&logoColor=white) 
![GIT](https://img.shields.io/badge/GIT-E44C30?style=for-the-badge&logo=git&logoColor=white)
![SQLite](https://img.shields.io/badge/Sqlite-003B57?style=for-the-badge&logo=sqlite&logoColor=white)

MonkeyChat! is a simple client-server chat written in pure C and GTK4 using sockets for data exchange between client and server

## Table of contents
[Dependencies](https://github.com/t1tandr/uchat#dependencies)\
[Building](https://github.com/t1tandr/uchat#buiding)\
[Usage](https://github.com/t1tandr/uchat#usage)\
[Credits](https://github.com/t1tandr/uchat#credits)\
[Screenshots](https://github.com/t1tandr/uchat#screenshots)\
[Links](https://github.com/t1tandr/uchat#links)\
[License](https://github.com/t1tandr/uchat#license)

## Dependencies

|Lib   |Version |
|---	|---	|
| GTK 	| 4.0+ 	|

## Buiding

```bash
git clone https://github.com/t1tandr/uchat.git
cd uchat/
make
```

## Usage

> [!WARNING]  
> App runs only on Linux OS.

```bash
git clone https://github.com/t1tandr/uchat.git
cd uchat/
make
```

Start server:
```bash
cd server/
./uchat-server 8080
```

Start client:
```bash
cd client/
./uchat localhost 8080
```

To shutdown server execute next command:
```bash
kill [server-pid]
```

## Credits

* [prov](https://github.com/Prov258)
* [neffarrty](https://github.com/neffarrty)
* [batushka](https://github.com/batushka11)
* [t1tandr](https://github.com/t1tandr)

## Screenshots
[![Screenshot-13.png](https://i.postimg.cc/brWk9BmW/Screenshot-13.png)](https://postimg.cc/n9KXH0bG)
[![Screenshot-14.png](https://i.postimg.cc/52wL8pLK/Screenshot-14.png)](https://postimg.cc/0zyzsYt7)
[![Screenshot-11.png](https://i.postimg.cc/PqyDRx6m/Screenshot-11.png)](https://postimg.cc/QBBVBXZt)
[![Screenshot-10.png](https://i.postimg.cc/tJZVJz95/Screenshot-10.png)](https://postimg.cc/RJm0bK66)
[![Screenshot-11.png](https://i.postimg.cc/PqyDRx6m/Screenshot-11.png)](https://postimg.cc/QBBVBXZt)

## Links

![YouTube](https://img.shields.io/badge/YouTube-FF0000?style=for-the-badge&logo=youtube&logoColor=white)

## License

MonkeyChat! is licensed under the [MIT](LICENSE) license.\
Copyright © 2024, OGURCI Team
