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

		public:
			CPacket(std::istream&& is) : m_MessageType(0) { is >> (*this); };
			CPacket(const unsigned short _MessageType) : m_MessageType(_MessageType) {};

		public:
			friend std::ostream& operator<<(std::ostream& os, const CPacket& Packet) {
				os.write(reinterpret_cast<const char*>(&Packet.m_MessageType), sizeof(Packet.m_MessageType));

				return os;
			}

			friend std::istream& operator>>(std::istream& is, CPacket& Packet) {
				is.read(reinterpret_cast<char*>(&Packet.m_MessageType), sizeof(Packet.m_MessageType));

				return is;
			}

		};

		/*struct CResult : public CPacket {
		public:
			unsigned short m_ResultMessage;
			bool m_bResult;

		public:
			CResult(std::istream& is) :CPacket(MessageType::EMESSAGETYPE::EMT_RESULT), m_ResultMessage(0), m_bResult(false) { is >> (*this); };
			CResult() : CPacket(MessageType::EMESSAGETYPE::EMT_RESULT), m_ResultMessage(0), m_bResult(false) {};
			CResult(unsigned short ResultMessage, bool bResult) : CPacket(MessageType::EMESSAGETYPE::EMT_RESULT), m_ResultMessage(ResultMessage), m_bResult(bResult) {};

		public:
			friend std::ostream& operator<<(std::ostream& os, const CResult& Result) {
				os.write(reinterpret_cast<const char*>(&Result.m_MessageType), sizeof(Result.m_MessageType));
				os.write(reinterpret_cast<const char*>(&Result.m_ResultMessage), sizeof(Result.m_ResultMessage));
				os.write(reinterpret_cast<const char*>(&Result.m_bResult), sizeof(Result.m_bResult));

				return os;
			}

			friend std::istream& operator>>(std::istream& is, CResult& Result) {
				is.read(reinterpret_cast<char*>(&Result.m_MessageType), sizeof(Result.m_MessageType));
				is.read(reinterpret_cast<char*>(&Result.m_ResultMessage), sizeof(Result.m_ResultMessage));
				is.read(reinterpret_cast<char*>(&Result.m_bResult), sizeof(Result.m_bResult));

				return is;
			}

		};*/

		struct CJoinPacket : public CPacket {
		public:
			uint16_t m_SessionID;

		public:
			CJoinPacket(uint16_t SessionID) : CPacket(MessageType::EMESSAGETYPE::EMT_JOIN), m_SessionID(SessionID) {};

		public:
			friend std::ostream& operator<<(std::ostream& os, const CJoinPacket& Packet) {
				os.write(reinterpret_cast<const char*>(&Packet.m_MessageType), sizeof(Packet.m_MessageType));
				os.write(reinterpret_cast<const char*>(&Packet.m_SessionID), sizeof(Packet.m_SessionID));

				return os;
			}

			friend std::istream& operator>>(std::istream& is, CJoinPacket& Packet) {
				is.read(reinterpret_cast<char*>(&Packet.m_MessageType), sizeof(Packet.m_MessageType));
				is.read(reinterpret_cast<char*>(&Packet.m_SessionID), sizeof(Packet.m_SessionID));

				return is;
			}

		};

		struct CFindPacket : public CPacket {
		public:
			std::vector<CSessionInfo> m_SessionInformation;

		public:
			CFindPacket() : CPacket(MessageType::EMESSAGETYPE::EMT_FIND) {};

		public:
			friend std::ostream& operator<<(std::ostream& os, const CFindPacket& Packet) {
				os.write(reinterpret_cast<const char*>(&Packet.m_MessageType), sizeof(Packet.m_MessageType));
				size_t Size = Packet.m_SessionInformation.size();
				os.write(reinterpret_cast<const char*>(&Size), sizeof(size_t));
				for (const auto& It : Packet.m_SessionInformation) {
					os << It;
				}

				return os;
			}

			friend std::istream& operator>>(std::istream& is, CFindPacket& Packet) {
				is.read(reinterpret_cast<char*>(&Packet.m_MessageType), sizeof(Packet.m_MessageType));
				size_t Size;
				is.read(reinterpret_cast<char*>(&Size), sizeof(Size));
				for (int i = 0; i < Size; i++) {
					CSessionInfo Info;
					is >> Info;
					Packet.m_SessionInformation.insert(Packet.m_SessionInformation.begin(), 0, Info);
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
				os.write(reinterpret_cast<const char*>(&Packet.m_MessageType), sizeof(Packet.m_MessageType));
				os << Packet.m_SessionInformation;

				return os;
			}

			friend std::istream& operator>>(std::istream& is, CCreatePacket& Packet) {
				is.read(reinterpret_cast<char*>(&Packet.m_MessageType), sizeof(Packet.m_MessageType));
				is >> Packet.m_SessionInformation;

				return is;
			}

		};

	}
}