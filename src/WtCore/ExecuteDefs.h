/*!
 * \file ExecuteDefs.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include "../Share/WTSDataDef.hpp"
#include "../Share/WTSCollection.hpp"

NS_OTP_BEGIN
class WTSTickData;
class WTSHisTickData;
class WTSVariant;
class WTSCommodityInfo;

typedef std::vector<uint32_t> OrderIDs;
typedef WTSMap<uint32_t> OrderMap;

//////////////////////////////////////////////////////////////////////////
//ִ�л���������
class ExecuteContext
{
public:
	ExecuteContext(){}
	virtual ~ExecuteContext(){}

public:
	/*
	 *	��ȡtick����
	 *	code	��Լ����
	 *	count	��������
	 *	etime	��ֹʱ��, 0��Ϊ��ǰʱ��
	 *	
	 *	����ֵ	��ʷ���ݷ�װ��ָ��
	 */
	virtual WTSHisTickData* getTicks(const char* stdCode, uint32_t count, uint64_t etime = 0) = 0;

	/*
	 *	��ȡ���һ��Tick����
	 *	code	��Լ����
	 *	
	 *	����ֵ	WTSTickDataָ��
	 */
	virtual WTSTickData*	grabLastTick(const char* stdCode) = 0;

	/*
	 *	��ȡ��λ
	 *	code	��Լ����
	 *	flag	������� 1-���, 2-�ղ�, 3-�����ƽ
	 *	
	 *	����ֵ	��ƽ��Ĳ�λ: ���>0, �ղ�<0
	 */
	virtual int32_t getPosition(const char* stdCode, int32_t flag = 3) = 0;

	/*
	 *	��ȡδ��ɶ���
	 *	code	��Լ����
	 *
	 *	����ֵ	localid-WTSOrderInfo��ӳ��
	 */
	virtual OrderMap* getOrders(const char* stdCode) = 0;

	/*
	 *	��ȡδ�������
	 *	code	��Լ����
	 *	
	 *	����ֵ	������ƽ�Ժ��δ�������
	 */
	virtual int32_t getUndoneQty(const char* stdCode) = 0;

	/*
	 *	����ӿ�
	 *	code	��Լ����
	 *	price	ί�м۸�, 0Ϊ�м۵�
	 *	qty		ί������
	 *
	 *	����ֵ	���ض���������: һ������������ܻ������3����������
	 */
	virtual OrderIDs buy(const char* stdCode, double price, uint32_t qty) = 0;

	/*
	*	�����ӿ�
	*	code	��Լ����
	*	price	ί�м۸�, 0Ϊ�м۵�
	*	qty		ί������
	*
	*	����ֵ	���ض���������: һ������������ܻ������3����������
	*/
	virtual OrderIDs sell(const char* stdCode, double price, uint32_t qty) = 0;

	/*
	 *	���ݱ��ض����ų���
	 *	localid ���ض�����
	 *
	 *	����ֵ	���س���ָ���Ƿ��ͳɹ�
	 */
	virtual bool	cancel(uint32_t localid) = 0;

	/*
	 *	����ָ���ķ������������
	 *	code ��Լ����
	 *	qty	���ٳ�������, ����ж��ί��, ��ô����ʱ��˳��һ��һ������, ֱ���������������ڵ���qty
	 *	���һ���ò���, ���Դ�0��ʱ��, ���ǰ�ȫ���򵥻�����������
	 *	isBuy ��or����
	 *	
	 *	����ֵ ����ʵ�ʷ����˳���ָ�������
	 */
	virtual OrderIDs cancel(const char* stdCode, bool isBuy, uint32_t qty = 0) = 0;

	/*
	 *	д��־
	 */
	virtual void writeLog(const char* fmt, ...) = 0;

	/*
	 *	��ȡƷ�ֲ���
	 */
	virtual WTSCommodityInfo* getCommodityInfo(const char* stdCode) = 0;

	/*
	 *	��ȡ��ǰʱ�䣬��ȷ�����룬��ʽ��20191127174139500
	 */
	virtual uint64_t	getCurTime() = 0;

	/*
	 *	ע�ᶨʱ��
	 *	@stdCode	��Լ����
	 *	@elapse		ʱ��������λ����
	 *
	 *	����ֵ		�Ƿ�ע��ɹ�
	 */
	virtual bool		registerTimer(const char* stdCode, uint32_t elapse){ return false; }
};

//////////////////////////////////////////////////////////////////////////
//ִ�е�Ԫ������
class ExecuteUnit
{
public:
	ExecuteUnit() :_ctx(NULL), _code(""){}
	virtual ~ExecuteUnit(){}

public:
	/*
	 *	ִ�е�Ԫ����
	 */
	virtual const char* getName() = 0;

	/*
	 *	����ִ������������
	 */
	virtual const char* getFactName() = 0;

	/*
	 *	��ʼ��ִ�е�Ԫ
	 *	ctx		ִ�е�Ԫ���л���
	 *	code	�����ĺ�Լ����
	 */
	virtual void init(ExecuteContext* ctx, const char* stdCode, WTSVariant* cfg){ _ctx = ctx; _code = stdCode; }

public:
	/*
	 *	�����µ�Ŀ���λ
	 *	code	��Լ����
	 *	newVol	�µ�Ŀ���λ
	 */
	virtual void set_position(const char* stdCode, int32_t newVol) = 0;

	/*
	 *	tick���ݻص�
	 *	newTick	���µ�tick����
	 */
	virtual void on_tick(WTSTickData* newTick) = 0;

	/*
	 *	�ɽ��ر�
	 *	code	��Լ����
	 *	isBuy	��or��
	 *	vol		�ɽ�����, ����û������, ͨ��isBuyȷ�����뻹������
	 *	price	�ɽ��۸�
	 */
	virtual void on_trade(const char* stdCode, bool isBuy, uint32_t vol, double price) = 0;

	/*
	 *	�����ر�
	 *	localid	���ص���
	 *	code	��Լ����
	 *	isBuy	��or��
	 *	leftover	ʣ������
	 *	price	ί�м۸�
	 *	isCanceled	�Ƿ��ѳ���
	 */
	virtual void on_order(uint32_t localid, const char* stdCode, bool isBuy, uint32_t leftover, double price, bool isCanceled) = 0;

	/*
	 *	
	 */
	virtual void on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message) = 0;

	/*
	 *	����ͨ�������ص�
	 */
	virtual void on_channel_ready() = 0;

	/*
	 *	����ͨ����ʧ�ص�
	 */
	virtual void on_channel_lost() = 0;

protected:
	ExecuteContext*	_ctx;
	std::string		_code;
};

//////////////////////////////////////////////////////////////////////////
//ִ�е�Ԫ�����ӿ�
typedef void(*FuncEnumUnitCallback)(const char* factName, const char* unitName, bool isLast);

class IExecuterFact
{
public:
	IExecuterFact(){}
	virtual ~IExecuterFact(){}

public:
	/*
	 *	��ȡ������
	 */
	virtual const char* getName() = 0;

	/*
	 *	ö��ִ�е�Ԫ
	 */
	virtual void enumExeUnit(FuncEnumUnitCallback cb) = 0;

	/*
	 *	�������ƴ���ִ�е�Ԫ
	 */
	virtual ExecuteUnit* createExeUnit(const char* name) = 0;

	/*
	 *	ɾ��ִ�е�Ԫ
	 */
	virtual bool deleteExeUnit(ExecuteUnit* unit) = 0;
};

//����ִ�й���
typedef IExecuterFact* (*FuncCreateExeFact)();
//ɾ��ִ�й���
typedef void(*FuncDeleteExeFact)(IExecuterFact* &fact);

NS_OTP_END