#include "fattester.h"
#include "usart.h"
#include "exfuns.h"
#include "../Malloc/malloc.h"
#include "ff.h"
#include "string.h"

//Ϊ����ע�Ṥ����
//path:����·��������"0:"��"1:"
//mt:0��������ע�ᣨ�Ժ�ע�ᣩ��1������ע��
//����ֵ:ִ�н��
uint8_t mf_mount(char *path,uint8_t mt)
{
    return f_mount(fs[0],(const TCHAR*)path,mt);
}
//��·���µ��ļ�
//path:·��+�ļ���
//mode:��ģʽ
//����ֵ:ִ�н��
uint8_t mf_open(char *path,uint8_t mode)
{
    uint8_t res;
    res=f_open(file,(const TCHAR*)path,mode);//���ļ���
    return res;
}
//�ر��ļ�
//����ֵ:ִ�н��
uint8_t mf_close(void)
{
    f_close(file);
    return 0;
}
//��������
//len:�����ĳ���
//����ֵ:ִ�н��
uint8_t mf_read(uint16_t len)
{
    uint16_t i,t;
    uint8_t res=0;
    uint16_t tlen=0;
    printf("\r\nRead file data is:\r\n");
    for(i=0; i<len/512; i++)
    {
        res=f_read(file,fatbuf,512,&br);
        if(res)
        {
            printf("Read Error:%d\r\n",res);
            break;
        } else
        {
            tlen+=br;
            for(t=0; t<br; t++)printf("%c",fatbuf[t]);
        }
    }
    if(len%512)
    {
        res=f_read(file,fatbuf,len%512,&br);
        if(res)	//�����ݳ�����
        {
            printf("\r\nRead Error:%d\r\n",res);
        } else
        {
            tlen+=br;
            for(t=0; t<br; t++)printf("%c",fatbuf[t]);
        }
    }
    if(tlen)printf("\r\nReaded data len:%d\r\n",tlen);//���������ݳ���
    printf("Read data over\r\n");
    return res;
}
//д������
//dat:���ݻ�����
//len:д�볤��
//����ֵ:ִ�н��
uint8_t mf_write(uint8_t*dat,uint16_t len)
{
    uint8_t res;

    printf("\r\nBegin Write file...\r\n");
    printf("Write data len:%d\r\n",len);
    res=f_write(file,dat,len,&bw);
    if(res)
    {
        printf("Write Error:%d\r\n",res);
    } else printf("Writed data len:%d\r\n",bw);
    printf("Write data over.\r\n");
    return res;
}

//��Ŀ¼
//path:·��
//����ֵ:ִ�н��
uint8_t mf_opendir(char* path)
{
    return f_opendir(&dir,(const TCHAR*)path);
}
//�ر�Ŀ¼
//����ֵ:ִ�н��
uint8_t mf_closedir(void)
{
    return f_closedir(&dir);
}
//���ȡ�ļ���
//����ֵ:ִ�н��
uint8_t mf_readdir(void)
{
    uint8_t res;
    char *fn;
#if _USE_LFN
    fileinfo.lfsize = _MAX_LFN * 2 + 1;
    fileinfo.lfname = mymalloc(fileinfo.lfsize);
#endif
    res=f_readdir(&dir,&fileinfo);//��ȡһ���ļ�����Ϣ
    if(res!=FR_OK||fileinfo.fname[0]==0)
    {
        myfree(fileinfo.lfname);
        return res;//������.
    }
#if _USE_LFN
    fn=*fileinfo.lfname ? fileinfo.lfname : fileinfo.fname;
#else
    fn=fileinfo.fname;;
#endif
    printf("\r\n DIR info:\r\n");

    printf("dir.id:%d\r\n",dir.id);
    printf("dir.index:%d\r\n",dir.index);
    printf("dir.sclust:%d\r\n",dir.sclust);
    printf("dir.clust:%d\r\n",dir.clust);
    printf("dir.sect:%d\r\n",dir.sect);

    printf("\r\n");
    printf("File Name is:%s\r\n",fn);
    printf("File Size is:%d\r\n",fileinfo.fsize);
    printf("File data is:%d\r\n",fileinfo.fdate);
    printf("File time is:%d\r\n",fileinfo.ftime);
    printf("File Attr is:%d\r\n",fileinfo.fattrib);
    printf("\r\n");
    myfree(fileinfo.lfname);
    return 0;
}

//�����ļ�
//path:·��
//����ֵ:ִ�н��
uint8_t mf_scan_files(char * path)
{
    FRESULT res;
    char *fn;   /* This function is assuming non-Unicode cfg. */
#if _USE_LFN
    fileinfo.lfsize = _MAX_LFN * 2 + 1;
    fileinfo.lfname = mymalloc(fileinfo.lfsize);
#endif

    res = f_opendir(&dir,(const TCHAR*)path); //��һ��Ŀ¼
    if (res == FR_OK)
    {
        printf("\r\n");
        while(1)
        {
            res = f_readdir(&dir, &fileinfo);                   //��ȡĿ¼�µ�һ���ļ�
            if (res != FR_OK || fileinfo.fname[0] == 0) break;  //������/��ĩβ��,�˳�
            //if (fileinfo.fname[0] == '.') continue;             //�����ϼ�Ŀ¼
#if _USE_LFN
            fn = *fileinfo.lfname ? fileinfo.lfname : fileinfo.fname;
#else
            fn = fileinfo.fname;
#endif	                                              /* It is a file. */
            printf("%s/", path);//��ӡ·��
            printf("%s\r\n",  fn);//��ӡ�ļ���
        }
    }
    myfree(fileinfo.lfname);
    return res;
}
//��ʾʣ������
//drv:�̷�
//����ֵ:ʣ������(�ֽ�)
uint32_t mf_showfree(char *drv)
{
    FATFS *fs1;
    uint8_t res;
    uint32_t fre_clust=0, fre_sect=0, tot_sect=0;
    //�õ�������Ϣ�����д�����
    res = f_getfree((const TCHAR*)drv,(DWORD*)&fre_clust, &fs1);
    if(res==0)
    {
        tot_sect = (fs1->n_fatent - 2) * fs1->csize;//�õ���������
        fre_sect = fre_clust * fs1->csize;			//�õ�����������
#if _MAX_SS!=512
        tot_sect*=fs1->ssize/512;
        fre_sect*=fs1->ssize/512;
#endif
        if(tot_sect<20480)//������С��10M
        {
            /* Print free space in unit of KB (assuming 512 bytes/sector) */
            printf("\r\n����������:%d KB\r\n"
                   "���ÿռ�:%d KB\r\n",
                   tot_sect>>1,fre_sect>>1);
        } else
        {
            /* Print free space in unit of KB (assuming 512 bytes/sector) */
            printf("\r\n����������:%d MB\r\n"
                   "���ÿռ�:%d MB\r\n",
                   tot_sect>>11,fre_sect>>11);
        }
    }
    return fre_sect;
}
//�ļ���дָ��ƫ��
//offset:����׵�ַ��ƫ����
//����ֵ:ִ�н��.
uint8_t mf_lseek(uint32_t offset)
{
    return f_lseek(file,offset);
}
//��ȡ�ļ���ǰ��дָ���λ��.
//����ֵ:λ��
uint32_t mf_tell(void)
{
    return f_tell(file);
}
//��ȡ�ļ���С
//����ֵ:�ļ���С
uint32_t mf_size(void)
{
    return f_size(file);
}
//����Ŀ¼
//pname:Ŀ¼·��+����
//����ֵ:ִ�н��
uint8_t mf_mkdir(char *pname)
{
    return f_mkdir((const TCHAR *)pname);
}
//��ʽ��
//path:����·��������"0:"��"1:"
//mode:ģʽ
//au:�ش�С
//����ֵ:ִ�н��
uint8_t mf_fmkfs(char* path,uint8_t mode,uint16_t au)
{
    return f_mkfs((const TCHAR*)path,mode,au);//��ʽ��,drv:�̷�;mode:ģʽ;au:�ش�С
}
//ɾ���ļ�/Ŀ¼
//pname:�ļ�/Ŀ¼·��+����
//����ֵ:ִ�н��
uint8_t mf_unlink(char *pname)
{
    return  f_unlink((const TCHAR *)pname);
}

//�޸��ļ�/Ŀ¼����(���Ŀ¼��ͬ,�������ƶ��ļ�Ŷ!)
//oldname:֮ǰ������
//newname:������
//����ֵ:ִ�н��
uint8_t mf_rename(char *oldname,char* newname)
{
    return  f_rename((const TCHAR *)oldname,(const TCHAR *)newname);
}
//��ȡ�̷����������֣�
//path:����·��������"0:"��"1:"
void mf_getlabel(char *path)
{
    uint8_t buf[20];
    uint32_t sn=0;
    uint8_t res;
    res=f_getlabel ((const TCHAR *)path,(TCHAR *)buf,(DWORD*)&sn);
    if(res==FR_OK)
    {
        printf("\r\n����%s ���̷�Ϊ:%s\r\n",path,buf);
        printf("����%s �����к�:%X\r\n\r\n",path,sn);
    } else printf("\r\n��ȡʧ�ܣ�������:%X\r\n",res);
}
//�����̷����������֣����11���ַ�������֧�����ֺʹ�д��ĸ����Լ����ֵ�
//path:���̺�+���֣�����"0:ALIENTEK"��"1:OPENEDV"
void mf_setlabel(char *path)
{
    uint8_t res;
    res=f_setlabel ((const TCHAR *)path);
    if(res==FR_OK)
    {
        printf("\r\n�����̷����óɹ�:%s\r\n",path);
    } else printf("\r\n�����̷�����ʧ�ܣ�������:%X\r\n",res);
}

//���ļ������ȡһ���ַ���
//size:Ҫ��ȡ�ĳ���
void mf_gets(uint16_t size)
{
    TCHAR* rbuf;
    rbuf=f_gets((TCHAR*)fatbuf,size,file);
    if(*rbuf==0)return  ;//û�����ݶ���
    else
    {
        printf("\r\nThe String Readed Is:%s\r\n",rbuf);
    }
}
//��Ҫ_USE_STRFUNC>=1
//дһ���ַ����ļ�
//c:Ҫд����ַ�
//����ֵ:ִ�н��
uint8_t mf_putc(char c)
{
    return f_putc((TCHAR)c,file);
}
//д�ַ������ļ�
//c:Ҫд����ַ���
//����ֵ:д����ַ�������
uint8_t mf_puts(char *c)
{
    return f_puts((TCHAR*)c,file);
}
