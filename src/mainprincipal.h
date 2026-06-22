#include "kosmos.h"
#include "resource.h"
#include <stdio.h>

typedef struct AppWidgets{
    KWidget texto;
    KWidget digtar;
    KWidget confirmar;
    KWidget combobox;
    KWidget commandlink;
    KWidget picker;
    KWidget calendar;
    KWidget listbox;
    KWidget listV;
    KWidget picture;
    KWidget progressbar;
    KWidget radio1;
    KWidget richedit;
    KWidget tab;
    KWidget ok;
    KWidget treeview;
} AppWidgets;

AppWidgets w;
KTabGroup grupoAbas;

kcontroller mainPrincipal(KWidget janela, KEvent evento, Kid id, KData data){
    
    switch (evento){
        case KInit: 
            w.texto = kGetWidget(janela, TEXTO);
            w.digtar = kGetWidget(janela, DIGTAR);
            w.confirmar = kGetWidget(janela, CONFIRMAR);
            w.combobox = kGetWidget(janela, COMBOFELIZ);
            w.commandlink = kGetWidget(janela, COMMAND1);
            w.picker = kGetWidget(janela, PICKER);
            w.calendar = kGetWidget(janela, CALENDAR);
            w.listbox = kGetWidget(janela, LIST);
            w.listV = kGetWidget(janela, LISTV);
            w.picture = kGetWidget(janela, IMAGE);
            w.progressbar = kGetWidget(janela, PROGRESS);
            w.radio1 = kGetWidget(janela, RAD);
            w.richedit = kGetWidget(janela, RICHEDIT);
            w.tab = kGetWidget(janela, TAB);
            w.ok = kGetWidget(janela, OK);
            w.treeview = kGetWidget(janela, TREEV);

            kTabAdd(w.tab, 0, "Aba 1");
            kTabAdd(w.tab, 1, "Aba 2");
            kTabAdd(w.tab, 2, "Aba 3");

            kTabBindGroups(&grupoAbas, w.tab, 3, w.richedit, w.ok, w.treeview);

            kTreeClear(w.treeview);
            KTreeItem raiz1 = kTreeAdd(w.treeview, KTREE_ROOT, "Raiz 1");
            kTreeAdd(w.treeview, raiz1, "Filho 1.1");
            kTreeAdd(w.treeview, raiz1, "Filho 1.2");
            KTreeItem raiz2 = kTreeAdd(w.treeview, KTREE_ROOT, "Raiz 2");
            kTreeAdd(w.treeview, raiz2, "Filho 2.1");
            kTreeAdd(w.treeview, raiz2, "Filho 2.2");

            

            kRadioSetCheck(w.radio1, TRUE);
            kProgressSetRange(w.progressbar, 0, 100);
            kProgressSetPos(w.progressbar, 50);

            kComboClear(w.combobox);
            kComboAdd(w.combobox, "Opção 1"); 
            kComboAdd(w.combobox, "Opção 2"); 

            kRichAppendText(w.richedit, "Bem-vindo ao Kosmos UI!", RGB(0, 128, 255));
            kPictureSetImageFile(w.picture, "resource/pao.jpg");
            
            kCommandLinkSetTitle(w.commandlink, "Título do Command Link");
            kCommandLinkSetNote(w.commandlink, "Nota do Command Link");

            kListAdd(w.listbox, "Item 1");
            kListAdd(w.listbox, "Item 2");
            
            kSetWidgetFont(w.listbox, "Audiowide", 30, TRUE, FALSE, FALSE);

            kListColAdd(w.listV, 0, "Registrador", 100);
            kListColAdd(w.listV, 1, "Valor Hex", 80);
            kListColAdd(w.listV, 2, "Status", 90);
            
            // Alimenta linhas de exemplo
            kListRowClear(w.listV);
            
            kListRowAdd(w.listV, 0, "EAX");
            kListSubSetText(w.listV, 0, 1, "0x0000FF12");
            kListSubSetText(w.listV, 0, 2, "Ativo");
            
            kListRowAdd(w.listV, 1, "EBX");
            kListSubSetText(w.listV, 1, 1, "0x00000000");
            kListSubSetText(w.listV, 1, 2, "Ocioso");
            
            kSetWidgetFont(w.texto, "Audiowide", 30, TRUE, FALSE, FALSE);
            return 1;
        break;
        
        case KCommand: {
            int widget = kGetId(id);
            switch (widget) {
                case CONFIRMAR: {
                    char buffer[256]; 
                    kGetText(w.digtar, buffer, 256);
                    kSetText(w.texto, buffer);
                    break;
                }
                case OK: {
                    KDateValue dataPicker = kGetDatePicker(w.picker);
                    KDateValue dataCalendar = kGetMonthCalendar(w.calendar);
                    
                    char info[512]; 
                    
                    sprintf(info, "Data do Picker: %02d/%02d/%04d\nData do Calendar: %02d/%02d/%04d",
                             dataPicker.day, dataPicker.month, dataPicker.year,
                             dataCalendar.day, dataCalendar.month, dataCalendar.year);
                    kSetText(w.texto, info);
                    break;
                }
                case LIST: {
                    int indexSelecionado = kListGetIndex(w.listbox, id);
                    if (indexSelecionado != -1) {
                        char itemTexto[256]; 
                        kListGetText(w.listbox, id, itemTexto, 256);
                        kSetText(w.texto, itemTexto);
                    } else {
                        kSetText(w.texto, "Nenhum item selecionado");
                    }
                    break;
                }
                case RAD: {
                    BOOL checado = kRadioGetCheck(w.radio1);
                    kMessageBox(janela, checado ? "Radio Button está selecionado" : "Radio Button não está selecionado", "Status do Radio Button");
                    break;
                }
            }
            return 1;
            break;
        }
        
        case KNotify: {
            int linha = kListViewGetIndex(w.listV, data);
            if (linha != -1) {
                char itemTexto[256]; 
                sprintf(itemTexto, "Item da ListView selecionado: Linha %d", linha + 1);
                kMessageBox(janela, itemTexto, "Notificação");
            }
            if (kTabManageEvents(&grupoAbas, data)) {
                return 1;
            }
            return 1;
            break;
        }
        
        case KClose:
            EndKosmos(janela);
            return 1;
            break;
   }
   return 0;
}