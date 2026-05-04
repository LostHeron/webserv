/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   default_pages.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 18:13:06 by jweber            #+#    #+#             */
/*   Updated: 2026/04/15 18:16:33 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFAULT_PAGES_HPP
# define DEFAULT_PAGES_HPP

# define ERROR_PAGE_400 "HTTP/1.1 400 Bad Request\r\n"\
"Server: webserv\r\n"\
"Date: Wed, 15 Apr 2026 16:12:58 GMT CHANGE_DATE\r\n"\
"Content-Type: text/html\r\n"\
"Content-Length: 147\r\n"\
"Connection: close\r\n"\
"\r\n"\
"<html>\r\n"\
"<head><title>400 Bad Request</title></head>\r\n"\
"<body>\r\n"\
"<center><h1>400 Bad Request</h1></center>\r\n"\
"<hr><center>webserv</center>\r\n"\
"</body>\r\n"\
"</html>\r\n"\

#endif
