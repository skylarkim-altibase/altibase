/** 
 *  Copyright (c) 1999~2017, Altibase Corp. and/or its affiliates. All rights reserved.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License, version 3,
 *  as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
 

/***********************************************************************
 * $Id: smaLogicalAger.h 82075 2018-01-17 06:39:52Z jina.kim $
 **********************************************************************/

#ifndef _O_SMA_LOGICAL_AGER_H_
# define _O_SMA_LOGICAL_AGER_H_ 1


# include <idtBaseThread.h>

# include <smDef.h>

# include <smm.h>
# include <idu.h>
# include <smaDef.h>

class smaLogicalAger : public idtBaseThread
{
    friend class smaDeleteThread;
    friend class smaFT;

 private:
    idBool       mContinue;
    UInt         mTimeOut;

    static smmSlotList* mSlotList[3];
    static iduMutex     mBlock;
    static iduMutex     mCheckMutex;
    static smaOidList*  mHead;
    static smaOidList*  mTailLogicalAger;
    static smaOidList*  mTailDeleteThread;

    static smaLogicalAger *mLogicalAgerList;
    // ������ Logical Ager Thread�� ����
    static UInt            mCreatedThreadCount;
    
    // ���� �������� Logical Ager Thread�� ����
    static UInt            mRunningThreadCount;

    /* BUG-17417 V$Ager������ Add OID������ ���� Ager��
     *           �ؾ��� �۾��� ������ �ƴϴ�.
     *
     * mMetaMtx�� �߰���. mAgingRequestOIDCntf�� ���ÿ�
     * Transaction�� �����ϴ� ���� �����ϱ� ����. */
    static iduMutex        mMetaMtxForAddRequest;

    /* mMetaMtx�� �߰���. mAgingProcessOIDCntf�� ���ÿ�
     * Transaction�� �����ϴ� ���� �����ϱ� ����. */
    static iduMutex        mMetaMtxForAddProcess;

    // Ager�� �ʱ�ȭ �Ǿ����� ����
    static idBool          mIsInitialized;

    //BUG-17371 [MMDB] Aging�� �и���� System�� ����ȭ �� Aging��
    //                 �и��� ������ �� ��ȭ��
    // getMinSCN ������, MinSCN ������ �۾����� ���� Ƚ��
    static ULong           mSleepCountOnAgingCondition;

    /* Logical Ager Thread������ �����Ϸ��� Thread��
     * ȹ���ؾ� �ϴ� Mutex
     */
    static iduMutex        mAgerCountChangeMutex;

    /* BUG-17474 Index Free Node List�� ���� ���ü� ��� �ǰ� ����
     *           �ʽ��ϴ�. */
    static iduMutex        mFreeNodeListMutex;
    /* TASK-4990 changing the method of collecting index statistics
     * FreeNode�� ���´�. Node��Ȱ���� ���Ƽ� Scan�� �����ϰ� �ϱ� �����̴�.*/
    static SInt            mBlockFreeNodeCount;

    /* BUG-35179 Add property for parallel logical ager */
    static UInt            mIsParallelMode;

    /* BUG-35179 Add property for parallel logical ager 
     * drop table, drop tablespace���� ���ü� ������ �ذ��ϱ� ���� �� thread����
     * lock�� �߰��Ѵ�. */
    iduMutex               mWaitForNoAccessAftDropTblLock;

    smSCN                  mViewSCN;

public:
    /* Transaction�� Commmit/Abort�� Aging List�� �ѱ�µ�
     * �̶� 1������ */
    static ULong  mAddCount;
    /* Ager�� List�� �ϳ��� ó���ϴµ� �̶� 1������ */
    static ULong  mHandledCount;

    /* BUG-17417 V$Ager������ Add OID������ ���� Ager��
     *           �ؾ��� �۾��� ������ �ƴϴ�.
     *
     * mAgingRequestOIDCnt, mAgingProcessedOIDCnt
     * �߰���.  */

    /* Transaction�� Commmit/Abort�� OID List�� Aging�� �����ؾ���
     * OID������ ��������.*/
    static ULong  mAgingRequestOIDCnt;

    /* Ager�� OID List�� OID�� �ϳ��� ó���ϴµ� �̶� 1������ */
    static ULong  mAgingProcessedOIDCnt;

public:
    static IDE_RC initializeStatic();
    static IDE_RC destroyStatic();
    static IDE_RC shutdownAll();
    
    smaLogicalAger();
    
    IDE_RC initialize( UInt aThreadID );
    
    IDE_RC destroy( void );
    
    IDE_RC shutdown( void );
    
    static IDE_RC setAger( idBool aValue);

    // Ager�� �ʱ�ȭ�Ǿ����� ���θ� �����Ѵ�.
    static idBool isInitialized( void );
    
    static IDE_RC block( void );
    static IDE_RC unblock( void );

    static IDE_RC lock() { return mBlock.lock( NULL /* idvSQL* */); }
    static IDE_RC unlock() { return mBlock.unlock(); }
    
    static IDE_RC lockCheckMtx( void ) { return mCheckMutex.lock( NULL /* idvSQL* */); }
    static IDE_RC unlockCheckMtx( void ) { return mCheckMutex.unlock(); }

    static IDE_RC lockMetaAddReqMtx() { return mMetaMtxForAddRequest.lock( NULL /* idvSQL* */); }
    static IDE_RC unlockMetaAddReqMtx() { return mMetaMtxForAddRequest.unlock(); }

    static IDE_RC lockMetaAddProcMtx() { return mMetaMtxForAddProcess.lock( NULL /* idvSQL* */); }
    static IDE_RC unlockMetaAddProcMtx() { return mMetaMtxForAddProcess.unlock(); }
    
    static IDE_RC lockChangeMtx() { return mAgerCountChangeMutex.lock( NULL /* idvSQL* */); }
    static IDE_RC unlockChangeMtx() { return mAgerCountChangeMutex.unlock(); }

    
    static IDE_RC addList(smTID         aTID,
                          idBool        aIsDDL,
                          smSCN         *aSCN,
                          smLSN         *aLSN,
                          UInt          aCondition,
                          smxOIDList*   aList,
                          void**        aAgerListPtr);

    static void setOIDListFinished( void* aOIDList, idBool aFlag);

    // Aging�� OID�� ����Ű�� Index Slot�� �����Ѵ�.
    static IDE_RC deleteIndex(smaOidList            * aOIDList,
                              smaInstantAgingFilter * aAgingFilter,
                              UInt                  * aDidDelOldKey );

    // Instant Aging Filter�� �ʱ�ȭ�Ѵ� ( smaDef.h ���� )
    static void initInstantAgingFilter( smaInstantAgingFilter * sAgingFilter );

    // Aging Filter���ǿ� �����ϴ��� üũ
    static idBool isAgingFilterTrue( smaInstantAgingFilter * aAgingFilter,
                                     scSpaceID               aTBSID,
                                     smOID                   aTableOID );
    
    
    // Ư�� Tablespace�� ���� OID�� ���ؼ� ��� Aging�ǽ�
    static IDE_RC doInstantAgingWithTBS( scSpaceID aTBSID );
    
    // Ư�� Table�� ���� OID�� ���ؼ� ��� Aging�ǽ�
    static IDE_RC doInstantAgingWithTable( smOID aTableOID );
    
    // smaPhysicalAger���ſ� ���� �߰��Լ���.
    static void addFreeNodes(void*      aFreeNodeList,
                             smnNode*   aFreeNodes);
    /* TASK-4990 changing the method of collecting index statistics
     * FreeNode�� ���´�. Node��Ȱ���� ���Ƽ� TreeLatch ����
     * Node�� FullScan �ϱ� �����̴�. 
     * �ٸ� ���ÿ� RebuildStat�� ���� ����� �� �����Ƿ�,
     * Block ���θ� Count�� ��ȸ�Ѵ�. */
    static void blockFreeNode()
    {
        IDE_ASSERT( mBlockFreeNodeCount >= 0 );
        IDE_ASSERT( mFreeNodeListMutex.lock( NULL ) == IDE_SUCCESS );
        mBlockFreeNodeCount ++;
        IDE_ASSERT( mFreeNodeListMutex.unlock() == IDE_SUCCESS );
    }

    static void unblockFreeNode()
    {
        IDE_ASSERT( mFreeNodeListMutex.lock( NULL ) == IDE_SUCCESS );
        mBlockFreeNodeCount --;
        IDE_ASSERT( mFreeNodeListMutex.unlock() == IDE_SUCCESS );
        IDE_ASSERT( mBlockFreeNodeCount >= 0 );
    }

    
    static void freeNodesIfPossible();
    
    static IDE_RC freeOldKeyFromIndexes(smcTableHeader*  aTableHeader,
                                        smOID            aOldVersionRowOID,
                                        UInt*            aDidDelOldKey);
    
    static IDE_RC checkOldKeyFromIndexes(smcTableHeader * aTableHeader,
                                         smOID            aOldVersionRowOID,
                                         idBool         * aExistKey );
    
    static void waitForNoAccessAftDropTbl();
    
    virtual void run();

    /*********************************************************************
     * Description : aOID�� dummy OID���� ���´�.
     *
     *     BUG-15306�� ���� �߰��� �Լ���
     *********************************************************************/
    static inline idBool isDummyOID( smaOidList* aOID )
    {
        if( ( aOID->mTransID   == 0 ) &&
            ( aOID->mCondition == 0 ) &&
            ( SM_SCN_IS_INIT( aOID->mSCN ) ) &&
            ( SM_IS_LSN_INIT( aOID->mLSN ) ) )
        {
            return ID_TRUE;
        }
        else
        {
            return ID_FALSE;
        }
    };

    static void addAgingRequestCnt( ULong aAgingRequestCnt );

    // Ager Thread�� ������ Ư�� ������ �ǵ��� ����,�����Ѵ�.
    static IDE_RC changeAgerCount( UInt aAgerCount );

private:
    // Ager Thread�� �ϳ� �߰��Ѵ�.
    static IDE_RC addOneAger();
    
    // Ager Thread�� �ϳ� �����Ѵ�.
    static IDE_RC removeOneAger();

    static IDE_RC addDummyOID( void );

    // Ư�� Table���� OID�� Ư�� Tablespace���� OID�� ����
    // ��� Aging�� �ǽ��Ѵ�.
    static IDE_RC agingInstantly( smaInstantAgingFilter * aAgingFilter );

    /* BUG-35179 Add property for parallel logical ager 
     * drop table, drop tablespace���� ���ü� ������ �ذ��ϱ� ���� �� thread����
     * lock�� �߰��Ѵ�. */

    inline IDE_RC lockWaitForNoAccessAftDropTbl()
    {
        return mWaitForNoAccessAftDropTblLock.lock( NULL /* idvSQL* */ );
    }

    inline IDE_RC unlockWaitForNoAccessAftDropTbl()
    {
        return mWaitForNoAccessAftDropTblLock.unlock();
    }
};

#endif /* _O_SMA_LOGICAL_AGER_H_ */