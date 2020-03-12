#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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
			std::string m_SessionName;
			uint16_t m_SessionID;
			uint16_t m_MaximumCount;
			uint16_t m_CurrentCount;
			bool m_bUsePassword;
			std::string m_Password;

		public:
			explicit CSessionInfo() : m_SessionID(0), m_MaximumCount(0), m_CurrentCount(0), m_bUsePassword(false) {};
			explicit CSessionInfo(uint16_t SessionID) : m_SessionID(SessionID), m_MaximumCount(0), m_CurrentCount(0), m_bUsePassword(false) {};
			explicit CSessionInfo(const std::string& SessionName, uint16_t MaximunCount, bool bUsePassword, const std::string& Password) : m_SessionName(SessionName), m_MaximumCount(MaximunCount), m_bUsePassword(bUsePassword), m_Password(Password), m_SessionID(0), m_CurrentCount(0) {};

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
			struct CAddress {
			public:
				unsigned short m_Port;
				unsigned long m_Address;

			public:
				explicit CAddress() : m_Port(0), m_Address(0) {};
#ifdef __cplusplus_cli
				explicit CAddress(unsigned short Port, unsigned long Address) : m_Port(Port), m_Address(Address) {};
#else if
				explicit CAddress(unsigned short Port, unsigned long Address) : m_Port(ntohs(Port)), m_Address(Address) {};
#endif

			public:
				friend std::ostream& operator<<(std::ostream& os, const CAddress& Address) {
					os.write(reinterpret_cast<const char*>(&Address.m_Port), sizeof(Address.m_Port));
					os.write(reinterpret_cast<const char*>(&Address.m_Address), sizeof(Address.m_Address));

					return os;
				}

				friend std::istream& operator>>(std::istream& is, CAddress& Address) {
					is.read(reinterpret_cast<char*>(&Address.m_Port), sizeof(Address.m_Port));
					is.read(reinterpret_cast<char*>(&Address.m_Address), sizeof(Address.m_Address));

					return is;
				}

			};

		public:
			CSessionInfo m_SessionInfo;
			std::vector<CAddress> m_UserAddresses;

		public:
			CJoinPacket(std::istream&& is) : CPacket(MessageType::EMESSAGETYPE::EMT_JOIN) { is >> (*this); }
			CJoinPacket(int ErrorCode) : CPacket(MessageType::EMESSAGETYPE::EMT_JOIN, ErrorCode) {}
			CJoinPacket(const CSessionInfo& SessionInfo) : CPacket(MessageType::EMESSAGETYPE::EMT_JOIN), m_SessionInfo(SessionInfo) {};
			CJoinPacket(const CSessionInfo& SessionInfo, const std::vector<CAddress>& Address) : CPacket(MessageType::EMESSAGETYPE::EMT_JOIN), m_SessionInfo(SessionInfo), m_UserAddresses(Address) {};

		public:
			friend std::ostream& operator<<(std::ostream& os, const CJoinPacket& Packet) {
				os << static_cast<const CPacket&>(Packet);
				os << Packet.m_SessionInfo;
				size_t vector_size = Packet.m_UserAddresses.size();
				os.write(reinterpret_cast<const char*>(&vector_size), sizeof(vector_size));
				for (auto Address : Packet.m_UserAddresses) {
					os << Address;
				}

				return os;
			}

			friend std::istream& operator>>(std::istream& is, CJoinPacket& Packet) {
				is >> static_cast<CPacket&>(Packet);
				is >> Packet.m_SessionInfo;
				size_t vector_size = 0;
				is.read(reinterpret_cast<char*>(&vector_size), sizeof(vector_size));
				for (size_t i = 0; i < vector_size; i++) {
					CJoinPacket::CAddress Address;
					is >> Address;
					Packet.m_UserAddresses.insert(Packet.m_UserAddresses.begin() + i, Address);
				}

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