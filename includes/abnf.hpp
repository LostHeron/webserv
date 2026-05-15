/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   abnf.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 15:20:11 by jweber            #+#    #+#             */
/*   Updated: 2026/04/15 17:19:51 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ABNF_HPP
# define ABNF_HPP

# define ABNF_UPPER "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
# define ABNF_LOWER "abcdefghijklmnopqrstuvwxyz"
# define ABNF_DIGIT "0123456789"
# define ABNF_ALPHA ABNF_UPPER ABNF_LOWER
# define ABNF_UNRESERVED ABNF_ALPHA ABNF_DIGIT "-" "." "_" "~"
# define ABNF_HEXDIG "ABCDEF" "abcdef" ABNF_DIGIT
# define ABNF_PCT_ENCODED "%" ABNF_HEXDIG ABNF_HEXDIG
# define ABNF_SUBDELIM "!" "$" "&" "'" "("  ")" "*" "+" "," ";" "="
# define ABNF_PCHAR	ABNF_UNRESERVED ABNF_PCT_ENCODED ABNF_SUBDELIM ":" "@"
# define ABNF_PATH_ABEMPTY "/" ABNF_PCHAR

#endif
