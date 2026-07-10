*This project has been created as part of the 42 curriculum by abetemps, cviel, jweber.*

# Description

The goal of this project is to build a server web.

The server web manages GET, PUT, POST, and DELETE request.

It manages CGI.

# Instructions

## Compilation

To compile the project just use the makefile provided at the root of the repository:

``make``

## Execution

After compiling the project (see above), you can start the server by running the following command :

``./webserv [configuration_file=./webserv.conf]``

The 'webserv' binary require a configuration file to start, if none is provided, it will try to use a 'webserv.conf' file in the current working directory.

## Configuration File

The configuration file format is json.

Here is an example of a working configuration file which should be exhaustive about everything the server handles:

```
"host":
[
	{
	    "listen":4444,
		"error":
		[
			{
				"code": 405,
				"link": "/paht/to/my/personnalized/page_405.html"
			},
			{
				"code": 403,
				"link": "/paht/to/my/personnalized/page_403.html"
			}
		],
	    "name": "host_a",
	    "root":"/www/a",
		"request":["GET"],
		"directory listing":true,
		"location":
	    [
	    	{
	    	    "name":"/images/",
	    	    "alias":"/www/images/",
				"max body": 1000000,
				"request":["GET","PUT","DELETE"]
	    	},
			{
				"name":"/cgi-bin/",
				"alias":"/www/cgi-bin/",
				"request":["GET", "POST", "PUT"],
				"cgi":true
			}
	    ]
	},
    {
	    "listen":3333,
	    "name": "host_b",
	    "root":"/www/b",
		"index": "index.html",
		"location":
	    [
	    	{
	    	    "name":"/images/",
	    	    "alias":"/www/images/"
	    	},
            {
                "name":"/cgi-bin/",
                "alias":"/www/cgi-bin/",
                "extension":[".sh", ".py"]
            },
			{
				"name":"/old_location",
				"redirection":"https://youtube.com"
			}
	    ]
    }
]
```

## Base of configuration file

Configuration file must start with a block of 'host':
```
"host":[...]
```

### Virtual host

in it each sub block will correspond to one 'virtual host'

```
"host":[
	{virtual host 1 spec},
	{virtual host 2 spec}, 
	..., 
	{last virtual host}
]
```
### Field in virtual host 

in each virtual host here are the allowed field:
- "listen"
- "root"
- "index"
- "directory listing"
- "request"
- "error"
- "location"
- "max body"

#### listen

Ports the virtual host will listen incoming connection

```
"listen": [444, 4253]
```

or this if there is only one port

```
"listen": 444
```

#### root

Select the root directory for the virtual host
```
"root": "/path/to/root"
```

#### index

Default file to look for if request target a directory
```
"index":"index.html"
```

#### directory listing

Activate or not directory listing is allowed or not (default: false)
```
"directory listing": true|false
```

#### request

Allow request amongst "GET", "POST", "PUT", "DELETE" (default : "GET" only)
this will allow all methods: (default: ["GET"])
```
"request":["GET", "PUT", "POST", "DELETE"]
```
this will allow none:
```
"request":[""]
```

#### error

Personnalize error page for certain error code:
```
"error":
[
	{
		"code": 405,
		"link": "/path/to/personnalized/for/error_405.html"
	},
	...,
	{
		"code": 403,
		"link": "/path/to/personnalized/for/error_403.html"
	}
]

```

#### max body

Specify max body size allowed in octets (default: ssize_t max)
```
"max body": 100 
```

#### location

Specify options specifically for the targeted location.

Possible options in a location
- "name"
- "alias"
- "max body"
- "cgi"
- "extension"
- "request" 
- "index"
- "upload"

example of locations:
```
location:
[
	{
		"name":"/images/",
		"alias":"/www/html/images/",
		...
	},
	...,
	{
		"name":"/cgi-bin",
		"alias":"/www/cgi-bin/",
		...
	}
]
```

##### name

Name of the target
```
"name":"/cgi-bin/"
```
all request starting with /cgi-bin/ will follow rules in the location block.

##### alias

New directory that the request should target (will replace the "root" in the virtual host) (default: "root")
```
"alias":"/real/path/on/the/machine/"
```

##### cgi

Wether all file in the directory should be considered as cgi script or not (default: false)
```
"cgi": true|false
```

##### extension

File ending with extension in the list should be considered script to be executed as CGI (default: [""])
```
"extension":[".sh",".py"]
```

##### upload

Where to put the resource from a "PUT" request (default: same as root)
```
"upload":"/path/to/location/to/upload/"
```

##### index

same as upper "index" but for the location matching "name"

##### max body

same as upper "max body" but for the location matching "name"

##### request

same as upper "request" but for the location matching "name"

# AI usage

AI was used to generate the blog post for testing the server web, and for generating redundant content, like default error pages.

# Resources

## References

- RFC 791 (IP) https://datatracker.ietf.org/doc/html/rfc791
- RFC 793 (TCP) https://datatracker.ietf.org/doc/html/rfc793
- RFC 1945 (HTTP/1.0) https://datatracker.ietf.org/doc/html/rfc1945
- RFC 2616 (HTTP/1.1) https://datatracker.ietf.org/doc/html/rfc2616
- RFC 3875 (CGI/1.1) https://datatracker.ietf.org/doc/html/rfc3875
- MDN Web Docs https://developer.mozilla.org/fr/

