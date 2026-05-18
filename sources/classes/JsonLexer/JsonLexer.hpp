/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonLexer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 17:10:23 by cviel             #+#    #+#             */
/*   Updated: 2026/05/07 17:36:29 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JSONLEXER_HPP
# define JSONLEXER_HPP

# include <string>
# include <queue>

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

		bool				empty(void) const;
		e_tokenType			peekType(void) const;
		std::string	const&	popToken(void);

	private:

		struct s_token
		{
			e_tokenType	type;
			std::string	value;	
		};

		std::queue<struct s_token>	_tokenQueue;

		JsonLexer(void);
};

#endif // JSONLEXER_HPP