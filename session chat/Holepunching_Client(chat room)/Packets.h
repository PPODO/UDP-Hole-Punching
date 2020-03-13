#pragma once
#include <sstream>
#ifdef __cplusplus_cli
#include <msclr/marshal_cppstd.h>
#endif

namespace Packets {
	namespace MessageType {
		enum EMESSAGETYPE : uint16_t {
			EMT_CONNECT = 1,
			EMT_JOIN,
			EMT_FIND,
			EMT_CREATE,
			EMT_RESULT,
			EMT_DISCONNECT
		};
	}

	namespace ErrorCode {
		constexpr int ESUCCEED = 0;
		constexpr int EMAXIMUMNUMBER = 100;
	}

	namespace Types {
		struct CSessionInfo {
		public:
			struct CUserInfo {
			public:
				unsigned short m_Port;
				unsigned long m_Address;
				std::string m_Nickname;

			public:
				explicit CUserInfo() : m_Port(0), m_Address(0) {};
#ifdef __cplusplus_cli
				explicit CUserInfo(System::String^ Nickname) : m_Port(0), m_Address(0), m_Nickname(msclr::interop::marshal_as<std::string>(Nickname)) {};
				explicit CUserInfo(unsigned short Port, unsigned long Address, System::String^ Nickname) : m_Port(Port), m_Address(Address), m_Nickname(msclr::interop::marshal_as<std::string>(Nickname)) {};
#else if
				explicit CUserInfo(unsigned short Port, unsigned long Address, const std::string& Nickname) : m_Port(ntohs(Port)), m_Address(Address), m_Nickname(Nickname) {};
#endif

			public:
				friend std::ostream& operator<<(std::ostream& os, const CUserInfo& UserInfo) {
					os.write(reinterpret_cast<const char*>(&UserInfo.m_Port), sizeof(UserInfo.m_Port));
					os.write(reinterpret_cast<const char*>(&UserInfo.m_Address), sizeof(UserInfo.m_Address));
					size_t Length = UserInfo.m_Nickname.length();
					os.write(reinterpret_cast<const char*>(&Length), sizeof(size_t));
					os.write(UserInfo.m_Nickname.c_str(), Length + 1);

					return os;
				}

				friend std::istream& operator>>(std::istream& is, CUserInfo& UserInfo) {
					is.read(reinterpret_cast<char*>(&UserInfo.m_Port), sizeof(UserInfo.m_Port));
					is.read(reinterpret_cast<char*>(&UserInfo.m_Address), sizeof(UserInfo.m_Address));
					size_t Length = 0;
					is.read(reinterpret_cast<char*>(&Length), sizeof(size_t));
					std::unique_ptr<char[]> TempPtr(new char[Length + 1]);
					is.read(TempPtr.get(), Length + 1);
					UserInfo.m_Nickname = std::string(TempPtr.get());

					return is;
				}

			};
		public:
			std::string m_SessionName;
			uint16_t m_SessionID;
			uint16_t m_MaximumCount;
			uint16_t m_CurrentCount;
			bool m_bUsePassword;
			std::string m_Password;
			std::vector<CUserInfo> m_UsersInfo;

		public:
			explicit CSessionInfo() : m_SessionID(0), m_MaximumCount(0), m_CurrentCount(0), m_bUsePassword(false) {};
			explicit CSessionInfo(uint16_t SessionID, const CUserInfo& UserInfo) : m_SessionID(SessionID), m_MaximumCount(0), m_CurrentCount(0), m_bUsePassword(false) { m_UsersInfo.push_back(UserInfo); };
			explicit CSessionInfo(const std::string& SessionName, uint16_t MaximunCount, bool bUsePassword, const std::string& Password) : m_SessionName(SessionName), m_MaximumCount(MaximunCount), m_bUsePassword(bUsePassword), m_Password(Password), m_SessionID(0), m_CurrentCount(0) {};
			explicit CSessionInfo(const std::string& SessionName, uint16_t MaximunCount, bool bUsePassword, const std::string& Password, const CUserInfo& UserInfo) : m_SessionName(SessionName), m_MaximumCount(MaximunCount), m_bUsePassword(bUsePassword), m_Password(Password), m_SessionID(0), m_CurrentCount(0) { m_UsersInfo.push_back(UserInfo); };
			explicit CSessionInfo(const std::string& SessionName, uint16_t MaximunCount, bool bUsePassword, const std::string& Password, const std::vector<CUserInfo>& UsersInfo) : m_SessionName(SessionName), m_MaximumCount(MaximunCount), m_bUsePassword(bUsePassword), m_Password(Password), m_SessionID(0), m_CurrentCount(0), m_UsersInfo(UsersInfo) {};

		public:
			friend std::ostream& operator<<(std::ostream& os, const CSessionInfo& SessionInfo) {
				size_t SessionNameLength = SessionInfo.m_SessionName.length();
				os.write(reinterpret_cast<const char*>(&SessionNameLength), sizeof(size_t));
				os.write(SessionInfo.m_SessionName.c_str(), SessionNameLength);
				os.write(reinterpret_cast<const char*>(&SessionInfo.m_SessionID), sizeof(SessionInfo.m_SessionID));
				os.write(reinterpret_cast<const char*>(&SessionInfo.m_MaximumCount), sizeof(SessionInfo.m_MaximumCount));
				os.write(reinterpret_cast<const char*>(&SessionInfo.m_CurrentCount), sizeof(SessionInfo.m_CurrentCount));
				os.write(reinterpret_cast<const char*>(&SessionInfo.m_bUsePassword), sizeof(SessionInfo.m_bUsePassword));
				size_t PasswordLength = SessionInfo.m_Password.length();
				os.write(reinterpret_cast<const char*>(&PasswordLength), sizeof(size_t));
				os.write(SessionInfo.m_Password.c_str(), PasswordLength);
				size_t UserInfoCount = SessionInfo.m_UsersInfo.size();
				os.write(reinterpret_cast<const char*>(&UserInfoCount), sizeof(size_t));
				for (auto User : SessionInfo.m_UsersInfo) {
					os << User;
				}

				return os;
			}

			friend std::istream& operator>>(std::istream& is, CSessionInfo& SessionInfo) {
				{
					size_t SessionNameLength = 0;
					is.read(reinterpret_cast<char*>(&SessionNameLength), sizeof(size_t));
					std::unique_ptr<char[]> TempSessionNameBuffer(new char[SessionNameLength + 1]);
					is.read(TempSessionNameBuffer.get(), SessionNameLength);
					TempSessionNameBuffer.get()[SessionNameLength] = '\0';
					SessionInfo.m_SessionName = std::string(TempSessionNameBuffer.get());
				}

				is.read(reinterpret_cast<char*>(&SessionInfo.m_SessionID), sizeof(SessionInfo.m_SessionID));
				is.read(reinterpret_cast<char*>(&SessionInfo.m_MaximumCount), sizeof(SessionInfo.m_MaximumCount));
				is.read(reinterpret_cast<char*>(&SessionInfo.m_CurrentCount), sizeof(SessionInfo.m_CurrentCount));
				is.read(reinterpret_cast<char*>(&SessionInfo.m_bUsePassword), sizeof(SessionInfo.m_bUsePassword));

				{
					size_t PasswordLength = 0;
					is.read(reinterpret_cast<char*>(&PasswordLength), sizeof(size_t));
					std::unique_ptr<char[]> TempPasswordBuffer(new char[PasswordLength + 1]);
					is.read(TempPasswordBuffer.get(), PasswordLength);
					TempPasswordBuffer.get()[PasswordLength] = '\0';
					SessionInfo.m_Password = std::string(TempPasswordBuffer.get());
				}

				{
					size_t Size = 0;
					is.read(reinterpret_cast<char*>(&Size), sizeof(size_t));
					for (size_t i = 0; i < Size; i++) {
						CSessionInfo::CUserInfo UserInfo;
						is >> UserInfo;
						SessionInfo.m_UsersInfo.insert(SessionInfo.m_UsersInfo.begin() + i, UserInfo);
					}
				}

				return is;
			}

		};

		struct CPacket {
		public:
			unsigned short m_MessageType;
			int m_ErrorCode;

		public:
			CPacket(std::istream&& is) : m_MessageType(0) { is >> (*this); };
			CPacket(const unsigned short _MessageType) : m_MessageType(_MessageType), m_ErrorCode(0) {};
			CPacket(const unsigned short _MessageType, const int _ErrorCode) : m_MessageType(_MessageType), m_ErrorCode(_ErrorCode) {};

		public:
			friend std::ostream& operator<<(std::ostream& os, const CPacket& Packet) {
				os.write(reinterpret_cast<const char*>(&Packet.m_MessageType), sizeof(Packet.m_MessageType));
				os.write(reinterpret_cast<const char*>(&Packet.m_ErrorCode), sizeof(Packet.m_ErrorCode));

				return os;
			}

			friend std::istream& operator>>(std::istream& is, CPacket& Packet) {
				is.read(reinterpret_cast<char*>(&Packet.m_MessageType), sizeof(Packet.m_MessageType));
				is.read(reinterpret_cast<char*>(&Packet.m_ErrorCode), sizeof(Packet.m_ErrorCode));

				return is;
			}

		};

		struct CJoinPacket : public CPacket {
		public:
			CSessionInfo m_SessionInfo;

		public:
			CJoinPacket(std::istream&& is) : CPacket(MessageType::EMESSAGETYPE::EMT_JOIN) { is >> (*this); }
			CJoinPacket(int ErrorCode) : CPacket(MessageType::EMESSAGETYPE::EMT_JOIN, ErrorCode) {}
			CJoinPacket(const CSessionInfo& SessionInfo) : CPacket(MessageType::EMESSAGETYPE::EMT_JOIN), m_SessionInfo(SessionInfo) {};

		public:
			friend std::ostream& operator<<(std::ostream& os, const CJoinPacket& Packet) {
				os << static_cast<const CPacket&>(Packet);
				os << Packet.m_SessionInfo;

				return os;
			}

			friend std::istream& operator>>(std::istream& is, CJoinPacket& Packet) {
				is >> static_cast<CPacket&>(Packet);
				is >> Packet.m_SessionInfo;

				return is;
			}

		};

		struct CFindPacket : public CPacket {
		public:
			std::vector<CSessionInfo> m_SessionInformation;

		public:
			CFindPacket(std::istream&& is) : CPacket(MessageType::EMESSAGETYPE::EMT_FIND) { is >> (*this); };
			CFindPacket() : CPacket(MessageType::EMESSAGETYPE::EMT_FIND) {};
			CFindPacket(const std::vector<CSessionInfo>& SessionInfo) : CPacket(MessageType::EMESSAGETYPE::EMT_FIND), m_SessionInformation(SessionInfo) {};

		public:
			friend std::ostream& operator<<(std::ostream& os, const CFindPacket& Packet) {
				os << static_cast<const CPacket&>(Packet);
				size_t Size = Packet.m_SessionInformation.size();
				os.write(reinterpret_cast<const char*>(&Size), sizeof(size_t));
				for (const auto& It : Packet.m_SessionInformation) {
					os << It;
				}

				return os;
			}

			friend std::istream& operator>>(std::istream& is, CFindPacket& Packet) {
				is >> static_cast<CPacket&>(Packet);
				size_t Size;
				is.read(reinterpret_cast<char*>(&Size), sizeof(Size));
				for (int i = 0; i < Size; i++) {
					CSessionInfo Info;
					is >> Info;
					Packet.m_SessionInformation.insert(Packet.m_SessionInformation.begin() + i, Info);
				}

				return is;
			}

		};

		struct CCreatePacket : public CPacket {
		public:
			CSessionInfo m_SessionInformation;

		public:
			explicit CCreatePacket(std::istream&& is) : CPacket(MessageType::EMESSAGETYPE::EMT_CREATE) { is >> (*this); };
			explicit CCreatePacket(const CSessionInfo& SessionInfo) : CPacket(MessageType::EMESSAGETYPE::EMT_CREATE), m_SessionInformation(SessionInfo) {};

		public:
			friend std::ostream& operator<<(std::ostream& os, const CCreatePacket& Packet) {
				os << static_cast<const CPacket&>(Packet);
				os << Packet.m_SessionInformation;

				return os;
			}

			friend std::istream& operator>>(std::istream& is, CCreatePacket& Packet) {
				is >> static_cast<CPacket&>(Packet);
				is >> Packet.m_SessionInformation;

				return is;
			}

		};

	}
}