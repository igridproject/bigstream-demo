# BigStream Demotration

## Configuration
```
Control API :: http://demo.bs.igridproject.info/api
Storage API :: http://demo.bs.igridproject.info/api
```
Token :: Generate at -> [http://demo.bs.igridproject.info/token](http://demo.bs.igridproject.info/token)

## HTTP Push Channel
* URL Pattern :: http://demo.bs.igridproject.info/io/vo_name.appKey?key1=val1&key2=val2
* Example
  * Send msg=hello,name=kamron to vo `example` and appKey `ex01` 
```
GET :: http://demo.bs.igridproject.info/io/example.ex01?msg=hello&name=kamron
```

## Wiki
* Read Demo & Tutorial Wiki -> [Click Here](https://github.com/igridproject/bigstream-demo/wiki)
