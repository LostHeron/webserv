/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonLexer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 17:10:23 by cviel             #+#    #+#             */
/*   Updated: 2026/04/07 19:34:13 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __JSONLEXER_HPP__
# define __JSONLEXER_HPP__

#include <string>
#include <queue>

class JsonLexer
{
	public:

		enum e_tokenType
		{
			INT,
			BOOL_TRUE,
			BOOL_FALSE,
			STRING,
			COLON,
			COMMA,
			ARRAY_OPEN,
			ARRAY_CLOSE,
			OBJ_OPEN,
			OBJ_CLOSE
		};
	
		JsonLexer(std::string const& input);
		JsonLexer(JsonLexer const& other);
		~JsonLexer();

		JsonLexer&	operator=(JsonLexer const& other);

		e_tokenType	peekType(void) const;
		std::string	popToken(void);

	private:

		struct s_token
		{
			e_tokenType	type;
			std::string	value;	
		};

		JsonLexer(void);

		std::queue<struct s_token>	_tokenQueue;
};

#endif