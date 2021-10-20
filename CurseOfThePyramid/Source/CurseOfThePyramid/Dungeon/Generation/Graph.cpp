// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph.h"
#include "Algo/Reverse.h"
#include <queue> 

UGraph::UGraph()
	:Nodes{}
	, Connections{}
{
}

void UGraph::MakeConnection(int from, int to)
{
	// check if connection is made 
	const TArray<FConnection>& fromConnections = GetConnections(from);
	const TArray<FConnection>& toConnections = GetConnections(to);
	for (size_t i = 0; i < fromConnections.Num(); i++)
	{
		FConnection frConnection = fromConnections[i];
		for (size_t j = 0; j < toConnections.Num(); j++)
		{
			if (frConnection == toConnections[j])
			{
				return;
			}
		}
	}
	// if not add the connection
	FConnection connection{};
	connection.From = from;
	connection.To = to;
	Connections.Add(connection);
	
	connection.From = to;
	connection.To = from;
	Connections.Add(connection);

}

void UGraph::AddNode(int index)
{
	for (size_t i = 0; i < Nodes.Num(); i++)
	{
		if (index == Nodes[i].NodeID)
		{
			return;
		}
	}
	FGraphNode newNode{};
	newNode.NodeID = index;
	Nodes.Add(newNode);
}

TArray<FConnection> UGraph::GetConnections(int nodeId)const
{
	TArray<FConnection> connections{};
	for (size_t i = 0; i < Connections.Num(); i++)
	{
		if (nodeId == Connections[i].From
			|| nodeId == Connections[i].To)
		{
			connections.Add(Connections[i]);
		}
	}
	return connections;
}

TArray<FConnection> UGraph::GetFromConnections(int nodeId)const
{
	TArray<FConnection> connections{};
	for (size_t i = 0; i < Connections.Num(); i++)
	{
		if (nodeId == Connections[i].From)
		{
			connections.Add(Connections[i]);
		}
	}
	return connections;
}

void UGraph::BreakConnection(int from, int to)
{
	FConnection connection = GetConnection(from, to);
	Connections.Remove(connection);
}

void UGraph::BreakConnection(int NodeIndex)
{
	const TArray<FConnection>& connections = GetConnections(NodeIndex);
	for (size_t i = 0; i < connections.Num(); i++)
	{
		Connections.Remove(connections[i]);
	}
}

FConnection UGraph::GetConnection(int from, int to)const
{
	for (size_t i = 0; i < Connections.Num(); i++)
	{
		if (Connections[i].From == from && Connections[i].To == to)
		{
			return Connections[i];
		}
	}
	// return an invalid connection
	FConnection returnConnection{};
	return returnConnection;
}

// bfs from gpp course
bool UGraph::DoNodesConnectFBS(int from, int end, TArray<FGraphNode>& path)const
{
	path.Empty();
	FGraphNode destinationNode = GetNode(end);
	FGraphNode startNode = GetNode(from);

	std::queue<FGraphNode> openList;
	TMap<FGraphNode, FGraphNode> closedList{};
	openList.push(startNode);

	while (!openList.empty())
	{
		FGraphNode currentNode = openList.front();
		openList.pop();

		if (currentNode == destinationNode)
		{
			break;
		}
		const TArray<FConnection>& connections = GetConnections(currentNode.NodeID);
		for (auto con : connections)
		{
			FGraphNode nextNode = GetNode(con.To);
			if (closedList.Find(nextNode) == nullptr)
			{
				openList.push(nextNode);
				if (closedList.Find(nextNode) == nullptr)
				{
					closedList.Add(nextNode, currentNode);
				}
				else
				{
					closedList[nextNode] = currentNode;
				}
			}
		}
	}

	// we reached the goal node	
	// start backtracking 
	FGraphNode currentNode = destinationNode;
	while (!(currentNode == startNode))
	{
		path.Add(currentNode);
		if (closedList.Find(currentNode) == nullptr)
		{
			return false;
		}
		currentNode = closedList[currentNode];
	}
	path.Add(startNode);
	Algo::Reverse(path);
	return true;
}

FGraphNode UGraph::GetNode(int index)const
{
	for (size_t i = 0; i < Nodes.Num(); i++)
	{
		if (Nodes[i].NodeID == index)
		{
			return Nodes[i];
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("UGraph::GetNode node not found"));
	return FGraphNode{};
}

TArray<FGraphNode> UGraph::GetNodesFromConnections(const TArray<FConnection>& connections)
{
	TArray<FGraphNode> returnvalue{};
	for (size_t i = 0; i < connections.Num(); i++)
	{
		const FGraphNode& fromNode = GetNode(connections[i].From);
		if (returnvalue.Find(fromNode) == INDEX_NONE)
		{
			returnvalue.Add(fromNode);
		}
		const FGraphNode& toNode = GetNode(connections[i].To);
		if (returnvalue.Find(toNode) == INDEX_NONE)
		{
			returnvalue.Add(toNode);
		}
	}
	return returnvalue;
}