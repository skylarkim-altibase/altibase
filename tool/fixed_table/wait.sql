CREATE OR REPLACE VIEW ADM_WAITING_LOCK  AS
SELECT C.TABLE_NAME as table_name,
       A.TRANS_ID AS MY_TID,  
       A.LOCK_MODE AS MY_MODE,
       B.TRANS_ID AS WAITING_TID,  
       B.LOCK_MODE AS YOUR_MODE
FROM
     SYSTEM_.SYSX_TABLE_LOCK_ A ,
     SYSTEM_.SYSX_TABLE_LOCK_ B,
     SYSTEM_.SYS_TABLES_ C
WHERE A.TABLE_OID=B.TABLE_OID AND
      A.IS_GRANT = 0 AND
      B.IS_GRANT = 1 AND
      A.TABLE_OID = C.TABLE_OID;
