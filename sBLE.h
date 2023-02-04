/**
 * @file sBLE.h
 * @author silvio3105 (www.github.com/silvio3105)
 * @brief HM-10/HM-16 library header file.
 * 
 * @copyright Copyright (c) 2023, silvio3105
 * 
 */

/*
License

Copyright (c) 2023, silvio3105 (www.github.com/silvio3105)

Access and use of this Project and its contents are granted free of charge to any Person.
The Person is allowed to copy, modify and use The Project and its contents only for non-commercial use.
Commercial use of this Project and its contents is prohibited.
Modifying this License and/or sublicensing is prohibited.

THE PROJECT AND ITS CONTENT ARE PROVIDED "AS IS" WITH ALL FAULTS AND WITHOUT EXPRESSED OR IMPLIED WARRANTY.
THE AUTHOR KEEPS ALL RIGHTS TO CHANGE OR REMOVE THE CONTENTS OF THIS PROJECT WITHOUT PREVIOUS NOTICE.
THE AUTHOR IS NOT RESPONSIBLE FOR DAMAGE OF ANY KIND OR LIABILITY CAUSED BY USING THE CONTENTS OF THIS PROJECT.

This License shall be included in all methodal textual files.
*/


#ifndef _SBLE_H_
#define _SBLE_H_


/** \addtogroup sBLE
 * @{
 * Frameworkless simple BLE library for various BLE modules.
*/


// ----- INCLUDE FILES
#include			<stdint.h>
#include			<string.h>
#include			<stdio.h>

// ----- DEFINES
// RETURN CODES
#define SBLE_NOK					0
#define SBLE_OK						1

// HM-10/HM-16 DATASHEET STUFF
#define SBLE_HM_DISCONNECT_TIME			100 /**< @brief Minimum time between toggling system key pin for breaking connection. */
#define SBLE_HM_RESET_TIME				100 /**< @brief Minimum time between toggling reset pin. */


// ----- ENUMS
enum sBLE_action_t : uint8_t {
	CMD = 0,
	PIN = 1
};

enum sBLE_HM_adv_interval_t : char {
	ADV_100MS = '0',
	ADV_152_5MS = '1',
	ADV_211_25MS = '2',
	ADV_318_75MS = '3',
	ADV_417_5MS = '4',
	ADV_546_25MS = '5',
	ADV_760MS = '6',
	ADV_852_5MS = '7',
	ADV_1022_5MS = '8',
	ADV_1285MS = '9',
	ADV_2S = 'A',
	ADV_3S = 'B',
	ADV_4S = 'C',
	ADV_5S = 'D',
	ADV_6S = 'E',
	ADV_7S = 'F'
};

enum sBLE_HM_baud_t : char {
	BAUD_1200 = '0',
	BAUD_2400 = '1',
	BAUD_4800 = '2',
	BAUD_9600 = '3',
	BAUD_19200 = '4',
	BAUD_38400 = '5',
	BAUD_57600 = '6',
	BAUD_115200 = '7',
	BAUD_230400 = '8'
};

enum sBLE_HM_power_t : char {
	GAIN_n18DBM = '0',
	GAIN_n12DBM = '1',
	GAIN_n6DBM = '2',
	GAIN_n3DBM = '3',
	GAIN_n2DBM = '4',
	GAIN_n1 = '5',
	GAIN_0DBM = '6',
	GAIN_3DBM = '7'
};

enum sBLE_HM_adv_mode_t : char {
	NORMAL = '0',
	RELIAsBLE_HM_ = '1'
};

enum sBLE_HM_role_t : char {
	PERIPHERAL = '0',
	CENTRAL = '1'
};

enum sBLE_HM_auth_t : char {
	NO_PIN = '0',
	AUTH_NO_PIN = '1',
	AUTH_PIN = '2',
	AUTH_BOND = '3'
};

enum sBLE_HM_adv_type_t : char {
	ADV_SR_CONN = '0',
	ONLY_LAST_DEV = '1',
	ONLY_ADV_SR = '2',
	ONLY_ADV = '3'
};

enum sBLE_HM_gain_tx_t : char {
	NO_TX_GAIN = '0',
	HIGH_GAIN = '1'
};

enum sBLE_HM_gain_rx_t : char {
	NO_RX_GAIN = '0',
	OPEN_GAIN = '1'
};

enum sBLE_HM_work_t : char {
	START_AUTO = '0',
	START_MANUAL = '1'
};

enum sBLE_HM_mode_t : char {
	TX_MODE = '0',
	LIMITED_N_TX_MODE = '1',
	REMOTE_N_TX_MODE = '2'
};

enum sBLE_HM_notify_t : char {
	NOTI_DISAsBLE_HM_ = '0',
	NOTI_ENAsBLE_HM_ = '1'
};

enum sBLE_HM_auto_sleep_t : char {
	SLEEP_AUTO = '0',
	SLEEP_MANUAL = '1'
};


// ----- TYPEDEFS
typedef void (*TXHandler)(const char* str, uint8_t len);
typedef void (*RXHandler)(char* str, uint8_t len, const char stopChar);
typedef void (*pinHandler)(void);
typedef uint8_t (*pinStatusHandler)(void);


// ----- CLASSES
class sBLE_HM
{
	// PUBLIC STUFF
	public:
	// OBJECT CONSTRUCTORS AND DECONSTRUCTORS
	sBLE_HM(TXHandler UARTTX, RXHandler UARTRX, pinHandler sys, pinHandler r, pinStatusHandler st);
	~sBLE_HM(void);

	// METHOD DECLARATIONS
	uint8_t init(void);
	void disconnect(sBLE_action_t type);
	void reset(sBLE_action_t type);


	inline uint8_t ping(void)
	{
		return send("AT", 2, "OK", 2);
	}

	inline uint8_t isConnected(void)
	{
		return status();
	}

	inline uint8_t renew(void)
	{
		return send("AT+RENEW", 8, "OK", 2);
	}

	inline uint8_t getMac(char* output)
	{
		return sendReceive("AT+ADDR?\r\n", 10, 19, '\r', output, 7, 12);
	}

	inline uint8_t setName(const char* input)
	{
		return sendCmd("AT+NAME%s", input);
	}

	inline uint8_t getName(char* output)
	{
		// Answer will be: "OK+Get:Name\r\n" where maximum name length is 13 chars.
		return sendReceive("AT+NAME?\r\n", 10, 21, '\r', output, 7, 13);
	}

	inline uint8_t setAdvInterval(sBLE_HM_adv_interval_t interval)
	{
		return sendCmd("AT+ADVI%c", interval);
	}

	inline uint8_t getAdvInterval(sBLE_HM_adv_interval_t* interval)
	{
		return sendReceive("AT+ADVI?\r\n", 10, 9, '\r', (char*)interval, 7, 1);
	}

	inline uint8_t setAdvType(sBLE_HM_adv_type_t type)
	{
		return sendCmd("AT+ADTY%c", type);
	}

	inline uint8_t getAdvType(sBLE_HM_adv_type_t* type)
	{
		return sendReceive("AT+ADTY?\r\n", 10, 9, '\r', (char*)type, 7, 1);
	}

	inline uint8_t setBaud(sBLE_HM_baud_t baud)
	{
		return sendCmd("AT+BAUD%c", baud);
	}

	inline uint8_t setTXGain(sBLE_HM_gain_tx_t gain)
	{
		return sendCmd("AT+GAIT%c", gain);
	}

	inline uint8_t getTXGain(sBLE_HM_gain_tx_t* gain)
	{
		return sendReceive("AT+GAIT?\r\n", 10, 9, '\r', (char*)gain, 7, 1);
	}

	inline uint8_t setTXGain(sBLE_HM_gain_rx_t gain)
	{
		return sendCmd("AT+GAIN%c", gain);
	}

	inline uint8_t getTXGain(sBLE_HM_gain_rx_t* gain)
	{
		return sendReceive("AT+GAIN?\r\n", 10, 9, '\r', (char*)gain, 7, 1);
	}

	inline uint8_t setWorkType(sBLE_HM_work_t type)
	{
		return sendCmd("AT+IMME%c", type);
	}

	inline uint8_t getWorkType(sBLE_HM_work_t* type)
	{
		return sendReceive("AT+IMME?\r\n", 10, 9, '\r', (char*)type, 7, 1);
	}

	inline uint8_t setMode(sBLE_HM_mode_t mode)
	{
		return sendCmd("AT+MODE%c", mode);
	}

	inline uint8_t getMode(sBLE_HM_mode_t* mode)
	{
		return sendReceive("AT+MODE?\r\n", 10, 9, '\r', (char*)mode, 7, 1);
	}

	inline uint8_t setConnNotify(sBLE_HM_notify_t type)
	{
		return sendCmd("AT+NOTI%c", type);
	}

	inline uint8_t getConnNotify(sBLE_HM_notify_t* type)
	{
		return sendReceive("AT+NOTI?\r\n", 10, 9, '\r', (char*)type, 7, 1);
	}

	inline uint8_t setPIN(const char* pin)
	{
		return sendCmd("AT+PASS%s", pin);
	}

	inline uint8_t getPIN(char* pin)
	{
		return sendReceive("AT+PASS?\r\n", 10, 14, '\r', pin, 7, 6);
	}

	inline uint8_t setSleepMode(sBLE_HM_auto_sleep_t mode)
	{
		return sendCmd("AT+PWRM%c", mode);
	}

	inline uint8_t getSleepMode(sBLE_HM_auto_sleep_t* mode)
	{
		return sendReceive("AT+PWRM?\r\n", 10, 9, '\r', (char*)mode, 7, 1);
	}

	inline uint8_t setPower(sBLE_HM_power_t power)
	{
		return sendCmd("AT+POWE%c", power);
	}

	inline uint8_t getPower(sBLE_HM_power_t* power)
	{
		return sendReceive("AT+POWE?\r\n", 10, 9, '\r', (char*)power, 7, 1);
	}

	inline uint8_t setAdvMode(sBLE_HM_adv_mode_t mode)
	{
		return sendCmd("AT+RELI%c", mode);
	}

	inline uint8_t getAdvMode(sBLE_HM_adv_mode_t* mode)
	{
		return sendReceive("AT+RELI?\r\n", 10, 9, '\r', (char*)mode, 7, 1);
	}

	inline uint8_t setRole(sBLE_HM_role_t role)
	{
		return sendCmd("AT+ROLE%c", role);
	}

	inline uint8_t getRole(sBLE_HM_role_t* role)
	{
		return sendReceive("AT+ROLE?\r\n", 10, 9, '\r', (char*)role, 7, 1);
	}

	inline void sleep(void)
	{
		send("AT+SLEEP", 8, "OK", 2);
	}

	inline void start(void)
	{
		send("AT+START", 8, "OK", 2);
	}

	inline uint8_t setAuthMode(sBLE_HM_auth_t mode)
	{
		return sendCmd("AT+TYPE%c", mode);
	}

	inline uint8_t getAuthMode(sBLE_HM_auth_t* mode)
	{
		return sendReceive("AT+TYPE?\r\n", 10, 9, '\r', (char*)mode, 7, 1);
	}


	// PRIVATE STUFF
	private:
	// VARIABLES
	TXHandler TX = nullptr;
	RXHandler RX = nullptr;
	pinHandler sysKey = nullptr;
	pinHandler rst = nullptr;
	pinStatusHandler status = nullptr;
	char rxBuff[32] = { '\0' };

	// METHOD DECLARATIONS
	uint8_t cmp(const char* input, const char* wanted, const uint8_t len);
	uint8_t send(const char* tx, uint8_t txLen, const char* rx, const uint8_t rxLen = 0);
	uint8_t sendReceive(const char* tx, const uint8_t txLen, const uint8_t rxLen, const char rxStop, char* output, const uint8_t rxOffset, const uint8_t rxMax);
	uint8_t sendCmd(const char* cmd, ...);
};

#endif // _SBLE_H_

// END WITH NEW LINE
