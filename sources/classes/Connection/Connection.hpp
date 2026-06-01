/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 21:55:35 by jweber            #+#    #+#             */
/*   Updated: 2026/06/01 22:55:49 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
# define CONNECTION_HPP

// so draft of this class:
// when a connection arrives,
// now the ListenSocket should create a
// Connection Structure.
// This Structure should have a InputSocket
// and an OutputSocket structure

#include "InCGI.hpp"
#include "InputSocket.hpp"
#include "OutCGI.hpp"
#include "OutputSocket.hpp"
#include "Server.hpp"
#include <netinet/in.h>
#include <stdint.h>

class Connection
{
	public:
		Connection(int fd, uint16_t localport, struct sockaddr_in& addr, Server& server);
		~Connection();

	private:
		Connection(const Connection& other);
		const Connection& operator=(const Connection& other);

		InputSocket		inputSocket;
		OutputSocket	outputSocket;

		InCGI			*inCGI;
		OutCGI			*outCGI;
};

// ok supposons que le InputSocket ne s'occupe plus que de lire depuis le socket si le buffer
// lequel il met la donné est vide.
// Si on garde le même principe que ce qui est retourné par epoll est un pointeur vers InputSocket,
// et que le inputSocket.process appelé ne fait que lire du socket et mettre ce lu dans un buffer,
// après il faut alors que dans la fonction .process, inputsocket appelle un fonction 
// ou une methode d'une classe ou autre, qui permet au buffer d'être process et vidé 
// au fur et à mesure qu'il est process
// mais quelle classe, une sorte de Request attribute ou autre
// et donc quand le inputSocket est initialisé, il doit être initialisé avec une structure
// et ce inputSocket devra appeler une fonction de la dite Classe, qui aura une référence 
// vers le buffer de inputSocket, et qui récupérera les données et les supprimera du buffer
// lorsque ce sera fait ?? 
// ça semble bien bordélique aussi.
// hm
//
// done le InputSocket:
// public:
//	std::string buffer;
//	
//	et on peut dire qu'il à un pointeur vers un class abstraite,
//	genre un
//	AStructure *class;
//	
//	et dans inputSocket.process,
//	inputSocket devra appeler quelque chose du style:
//	class.parse(buffer)
//	
//	ça peut être quelque chose dans le style,
//	et en l'occurence, class au début de la requêt serait genre un 
//	Requête, qui contiendrait toutes les données de la requête, une classe dans le style:
//	classe Requete:
//	{
//		public:
//			getter of the methods and uri and verion etc..
//		private:
//			std::string method;
//			std::string uri;
//			std::string version;
//			std::string headers;
//	}
//
//	ok and then when each field have been processed, the pointer of the AStructure should
//	be set to something else, another AStructure, which could be like.
//	
//	ok why not but then who's in charge of changing the class in charge of processing the buffer
//	of the input Socket ?
//
//	and are the AStructure classes some kind of statemachine at any point ?
//
//	so when we get to the body part, the intermediate function should be
//	the one responsible to transfer data from InputRequest buffer to InCGI buffer ??
//	--> no! the buffer must be shared between InputRequest and InCGI, so in this case,
//	when AStructure finshied parse the request meta data, function should  pointer should
//	be set to NULL, so that no one except the InCGI touches the buffer of the inputSocket.
//
// hmmmmmmmmmmmmmmmmmm. I do not see any elegant solution to make inputSocket only doing the job
// of retrieving the data. Let's let that part like an interrogation.
//
// and let's keep the model.
//
// so InputSocket get the data and get method, uri, version and headers.
//
// ok the wrong thing done actually is that we pass to the OutputSocket only when headers are 
// finished being parsed, but we should also pass it when we get some bad request stuff,
// and when we get so, we should have flag in InputSocket to stop process the data, even if
// client keeps sending data.
//
// and how to pass data to the OutputSocket, a function which should send, some data as buffer
// and some fd, either pipe in which we wrote the default error page, or an fd, referring to
// the personalized error page of the host
//
// that should be somethings to be done twomorrow, and so the status flag of the ASocket of the 
// InputSocket should not be terminate, but more like Stop processing, and a function
// called like well idk yet, but when we call it, we exit the 'process' function of
// InputSocket. 
//
// But I think the connection Structure is a good idea still, and a global flag in this structure
// and when we want to clear a connection, we do not do server.remove(ASocket*) anymore
// but more server.remove(Connection), which could be cleaner, so the connection has to clean it's
// own ressources instead of a ASocket keeping track of its linked connection.
//
// might be cleaner like that.
//
// so now, ASocket should keep track more of the Connection rather than the server,
// and maybe the connection should keep a reference of the Server class.
//
// That's some things that can be done twomomrrow. Hm maybe, lets see
//
// 1) a function which setup OutputSocket, and set a flags on InputSocket so that
// program stop enter 'process' function of InputSocket;
//
// nb: should buffered be kept in the connection class ?
// nb: how to setup the data in outputBuffer of the OutputSocket, 
// inputSocket keep tracks of its outputSocket ?? raaah that's still annoying
//
// 2) The connection class.

Connection::Connection(int fd, uint16_t localport, struct sockaddr_in& addr, Server& server):
	inputSocket(fd, localport, addr, server),
	outputSocket(fd, server)
{
}


#endif // !CONNECTION_HPP
