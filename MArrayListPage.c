#include <stdbool.h>
#include "MArrayListPage.h"
#include "init.h"
#include "currency.h"
#include "mtmm.h"

MArrayListPage *MArrayListPageInit(MTMemoryManager * mm, DataSizeType psize, DataSizeType Page) {
  MemoryInfo *jm = MTMemoryManagerCalloc(mm, sizeof(MArrayListPage));
  MemoryInfo *me = MTMemoryManagerCalloc(mm, psize * sizeof(MArrayList));
  for (DataSizeType i = 0; i < psize; i++) {
    ((MArrayList **) me->m)[i] = MArrayListInit(mm, Page);
  }
  MArrayListPage *malp = (MArrayListPage *) jm->m;
  malp->map = jm;
  malp->page = me;
  malp->psize = psize;
  malp->PageLength = Page;
  malp->Position = 0;
  return malp;
}

MArrayListPage *CreateMArrayListPage(MTMemoryManager * mm, MArrayList * mal,DataSizeType PageLength) {
  MemoryInfo *jm = MTMemoryManagerCalloc(mm, sizeof(MArrayListPage)),*me;
  MArrayListPage *malp = (MArrayListPage *) jm->m;
  if(PageLength<=mal->length) {
      DataSizeType s = mal->length / PageLength;
    if (mal->length - (s * PageLength) != 0) {
      s++;
    }
    me = MTMemoryManagerCalloc(mm, s * sizeof(MArrayList));
    MArrayList **al = (MArrayList **) me->m;
    for (DataSizeType i = 0; i < s - 1; i++) {
      al[i] = MArrayListInit(mm, PageLength);
      for (DataSizeType u = i * PageLength; u < (i + 1) * PageLength; u++) {
        MArrayListAddIndex(mm, al[i], MArrayListGetIndex(mal, u));
      }
    }
    al[s - 1] = MArrayListInit(mm, PageLength);
    for (DataSizeType i = (s - 1) * PageLength; i < mal->length; i++) {
      MArrayListAddIndex(mm, al[s - 1], MArrayListGetIndex(mal, i));
    }
    malp->map = jm;
    malp->page = me;
    malp->psize = s;
    malp->PageLength = PageLength;
    malp->Position = s - 1;
  }else{
    me = MTMemoryManagerCalloc(mm,sizeof(MArrayList));
    MArrayList **al = (MArrayList **) me->m;
    al[0] = MArrayListInit(mm, mal->length);
    for (DataSizeType u = 0; u < mal->length; u++) {
      MArrayListAddIndex(mm, al[0], MArrayListGetIndex(mal, u));
    }
    malp->map = jm;
    malp->page = me;
    malp->psize = 1;
    malp->PageLength = PageLength;
    malp->Position = 0;
  }
  return malp;
}

MArrayListPage *CreateMArrayListPage2(MTMemoryManager *mm,MArrayList *mal,DataSizeType HeadPage,DataSizeType PageLength){
  MemoryInfo *jm = MTMemoryManagerCalloc(mm, sizeof(MArrayListPage)),*me;
  MArrayListPage *malp = (MArrayListPage *) jm->m;
  if(HeadPage<=mal->length||PageLength<=mal->length){
      DataSizeType s = ((mal->length-HeadPage) / PageLength) + 1;
    if ((mal->length-HeadPage) % PageLength != 0) {
      s++;
    }
    me = MTMemoryManagerCalloc(mm, s * sizeof(MArrayList));
    MArrayList **al = (MArrayList **) me->m;
    al[0] = MArrayListInit(mm, PageLength);
    for (DataSizeType u = 0; u < HeadPage; u++) {
      MArrayListAddIndex(mm, al[0], MArrayListGetIndex(mal, u));
    }
    for (DataSizeType i = 0; i < s - 2; i++) {
      al[i + 1] = MArrayListInit(mm, PageLength);
      for (DataSizeType u = (i * PageLength)+HeadPage; u < ((i + 1) * PageLength)+HeadPage; u++) {
        MArrayListAddIndex(mm, al[i + 1], MArrayListGetIndex(mal, u));
      }
    }
    al[s - 1] = MArrayListInit(mm, mal->length - (((s - 2) * PageLength) + HeadPage));
    for (DataSizeType i = ((s - 2) * PageLength) + HeadPage; i < mal->length; i++) {
      MArrayListAddIndex(mm, al[s - 1], MArrayListGetIndex(mal, i));
    }
    malp->map = jm;
    malp->page = me;
    malp->psize = s;
    malp->PageLength = PageLength;
    malp->Position = s - 1;
  }else{
    me = MTMemoryManagerCalloc(mm,sizeof(MArrayList));
    MArrayList **al = (MArrayList **) me->m;
    al[0] = MArrayListInit(mm, mal->length);
    for (DataSizeType u = 0; u < mal->length; u++) {
      MArrayListAddIndex(mm, al[0], MArrayListGetIndex(mal, u));
    }
    malp->map = jm;
    malp->page = me;
    malp->psize = 1;
    malp->PageLength = PageLength;
    malp->Position = 0;
  }
  return malp;
}

bool MArrayListPageAddIndex(MTMemoryManager * mm, MArrayListPage * map, void *m) {
  for (DataSizeType i = map->Position; i < map->psize + 1; i++) {
    MArrayList *mall = ((MArrayList **) map->page->m)[i];
    if (mall->length == map->PageLength) {
      if (map->Position == map->psize - 1) {
        map->page =
          MTMemoryManagerRealloc(mm,
                               map->page,
                               (map->psize + 1) * sizeof(MArrayList));
        ((MArrayList **) map->page->m)[map->Position + 1] =
          MArrayListInit(mm, map->PageLength);
        map->psize++;
      }
      map->Position++;
    } else {
      break;
    }
  }
  return MArrayListAddIndex(mm, ((MArrayList **) map->page->m)[map->Position], m);
}

bool  MArrayListPageAddIndexs(MTMemoryManager * mm,MArrayListPage * map, MArrayList * m) {
   bool b=true;
  for (DataSizeType ii = 0; ii < MArrayListSize(m); ii++) {
    for (DataSizeType i = map->Position; i < map->psize + 1; i++) {
      MArrayList *mall = ((MArrayList **) map->page->m)[i];
      if (mall->length == map->PageLength) {
        if (map->Position == map->psize - 1) {
            DataSizeType s = (MArrayListSize(m) - i) / map->PageLength;
          if ((map->PageLength * s) != (MArrayListSize(m) - i)) {
            s++;
          }
          map->page =
            MTMemoryManagerRealloc(mm, map->page,
                                 (map->psize + s) * sizeof(MArrayList));
          for (DataSizeType y = 1; y < s; y++) {
            ((MArrayList **) map->page->m)[map->Position + y] =
              MArrayListInit(mm, map->PageLength);
          }
          map->psize += s;
        }
        map->Position++;
      } else {
        break;
      }
    }
    MArrayList *mal = ((MArrayList **) map->page->m)[map->Position];
    if(!MArrayListAddIndex(mm, mal, MArrayListGetIndex(m, ii))){
		b=false;
	}
  }
  return b;
}

bool MArrayListPageDeleteIndex(MArrayListPage * mal, DataSizeType page,DataSizeType i) {
  if(MArrayListDeleteAppointIndex2(((MArrayList **) mal->page->m)[page], i)){
  if (mal->Position > page) {
    mal->Position = page;
  }
  return true;
  }else{
	  return false;
  }
}

bool MArrayListPageDeleteIndex2(MArrayListPage *mal,void *m,Cmp c){
  MArrayList **mt=(MArrayList**)mal->page->m;
  bool b=false;
  for(DataSizeType i=0;i<mal->psize;i++){
	 if(MArrayListDeleteIndex2(mt[i],m,mcmp)){
		 b=true;
		 if (mal->Position > i) {
    mal->Position = i;
  }
		 break;
	 }
  }
  return b;
}

MArrayList **MArrayListPageGet(MArrayListPage * m) {
  return (MArrayList **) m->page->m;
}

DataSizeType MArrayListPageTotalSize(MArrayListPage * m) {
  return m->psize;
}

void MArrayListPageDestroy(MTMemoryManager * mm, MArrayListPage * mal) {
  for (DataSizeType i = 0; i < mal->psize; i++ ) {
    MArrayListDestroy(mm, ((MArrayList **) mal->page->m)[i]);
  }
  MTMemoryManagerFree(mm, mal->page);
  MTMemoryManagerFree(mm, mal->map);
}

void MArrayListPageDestroy2(MTMemoryManager * mm, MArrayListPage * mal) {
  for (DataSizeType i = 0; i < mal->psize; i++ ) {
    MArrayListDestroy2(mm, ((MArrayList **) mal->page->m)[i]);
  }
  MTMemoryManagerFree(mm, mal->page);
  MTMemoryManagerFree(mm, mal->map);
}
