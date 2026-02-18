int LoadConfig();
class RAGEConfig
{
public:
	struct Pekora
	{
		static inline bool RichPresence;
		static inline bool FpsUnlocker;
		static inline bool FurryMode;
	};
private:
	int SetupPekoraConfig();
};
