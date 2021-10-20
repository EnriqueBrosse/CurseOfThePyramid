// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LevelGeneratorStructs.h"
#include "Graph.generated.h"

/**
 * 
 */
UCLASS()
class CURSEOFTHEPYRAMID_API UGraph : public UObject
{
	GENERATED_BODY()
public:
	UGraph();
	void MakeConnection(int from, int to); 
	void BreakConnection(int from, int to);
	void BreakConnection(int NodeIndex);
	void AddNode(int index);
	TArray<FConnection> GetConnections(int nodeId)const;
	TArray<FConnection> GetFromConnections(int nodeId)const;
	FConnection GetConnection(int from, int to)const;
	FGraphNode GetNode(int index)const;
	bool DoNodesConnectFBS(int from, int end, TArray<FGraphNode>& path)const;

	TArray<FGraphNode> GetNodesFromConnections(const TArray<FConnection>& connections);

	FORCEINLINE const TArray<FConnection>& GetConnections()const { return Connections; };
	FORCEINLINE const TArray<FGraphNode>& GetNodes()const { return Nodes; };

	FORCEINLINE int GetRowSize()const { return RowSize; }
	FORCEINLINE int GetColumnSize()const { return ColumnSize; }

	void SetRowSize(int size) { RowSize = size; }
	void SetColumnSize(int size) { ColumnSize = size; }

private:
	UPROPERTY()
		TArray<FGraphNode> Nodes{};
	UPROPERTY()
		TArray<FConnection>Connections{};

	int RowSize{};
	int ColumnSize{};

};
