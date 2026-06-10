*This project has been created as part of the 42 curriculum by abetemps, cviel, jweber.

# Description

The goal of this project is to build a server web.

The server web manages GET, POST, and DELETE request.

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

``
"host":
[
	{
	    "listen":4444,
	    "name": "host_a",
	    "root":"/www/a",
		"location":
	    [
	    	{
	    	    "name":"/images",
	    	    "alias":"/home/jweber/goinfre/tmp/b/"
	    	}
	    ]
	},
    {
	    "listen":3333,
	    "name": "host_b",
	    "root":"/www/b",
		"location":
	    [
	    	{
	    	    "name":"/images",
	    	    "alias":"/www/images"
	    	},
            {
                "name":"/cgi-bin",
                "alias:"/www/cgi-bin",
                "cgi":true,
            },
            {
                "name":"/cgi-bin-extension",
                "alias":"/www/cgi-bin",
                "cgi-extension":
                [
                    ".php",
                    ".sh"
                ]
            }
	    ]
    }
]
``

# Resources

## References

- RFC 791 (IP) https://datatracker.ietf.org/doc/html/rfc791
- RFC 793 (TCP) https://datatracker.ietf.org/doc/html/rfc793
- RFC 1945 (HTTP/1.0) https://datatracker.ietf.org/doc/html/rfc1945
- RFC 2616 (HTTP/1.1) https://datatracker.ietf.org/doc/html/rfc2616
- RFC 3875 (CGI/1.1) https://datatracker.ietf.org/doc/html/rfc3875

## AI usage
