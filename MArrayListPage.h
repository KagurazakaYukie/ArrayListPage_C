#ifndef MARRAYLISTPAGE_H
#define MARRAYLISTPAGE_H

#include "MArrayList.h"
#include "mtmm.h"
#include "init.h"
#include "currency.h"
#include <stdbool.h>
#include <stdio.h>

typedef struct MALP_ {
    DataSizeType PageLength,Position;
    DataSizeType psize;
    MemoryInfo *page,*map;
} MArrayListPage;

MArrayListPage *MArrayListPageInit(MTMemoryManager *mm,DataSizeType Totalze,DataSizeType PageLeng);

MArrayListPage *CreateMArrayListPage(MTMemoryManager *mm,MArrayList *mal,DataSizeType PageLength);

MArrayListPage *CreateMArrayListPage2(MTMemoryManager *mm,MArrayList *mal,DataSizeType HeadPage,DataSizeType PageLength);

bool MArrayListPageAddIndex(MTMemoryManager *mm,MArrayListPage *map,void *m);

bool MArrayListPageAddIndexs(MTMemoryManager *mm,MArrayListPage *map,MArrayList *m);

bool MArrayListPageDeleteIndex(MArrayListPage *mal,DataSizeType i,DataSizeType ui);

bool MArrayListPageDeleteIndex2(MArrayListPage *mal,void *m,Cmp c);

MArrayList **MArrayListPageGet(MArrayListPage *m);

DataSizeType MArrayListPageTotalSize(MArrayListPage *m);

void MArrayListPageDestroy(MTMemoryManager *mm,MArrayListPage *mal);

void MArrayListPageDestroy2(MTMemoryManager *mm,MArrayListPage *mal);

#endif
