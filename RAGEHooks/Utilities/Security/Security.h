#pragma once
namespace RAGESecurity
{
	void CorruptMemory();
	bool CheckIfOutsideModule(void* returnAddress);
	void StartDetection();
}