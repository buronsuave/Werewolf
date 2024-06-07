#include "GameClient.h"

void GameClient::handle_recv(char buf[], SOCKET s)
{
    switch (stage)
    {
        case STAGE_LOBBY:
        {
            if (strstr(buf, GAME_EVENT_MAINHOST))
            {
                printf("You're the main host\n");
                printf("You have the power to start the play!\n");
                printf("Type your name, and when you get ready press enter to start the play.\n");
                char response[DEFAULT_BUFLEN];
                char name[DEFAULT_BUFLEN];
                strcpy(response, GAME_EVENT_INIT);
                std::cin.getline(name, DEFAULT_BUFLEN);
                strcat(response, name);

                send_message(s, response, DEFAULT_BUFLEN);
                return;
            }
            // TODO: lobby,jugador1,jugador2,jugador3
            else if (strstr(buf, GAME_EVENT_LOBBY)) // TODO: Checar si GAME_EVENT_LOBBY esta contenida en el buf
            {
                // TODO: MOSTRAR LOS JUGADORES QUE SE HAN UNIDO
                printf("You've just jumped to the lobby.\nWaiting for main host to start.\n");
                printf("Please enter your name: ");
                char response[DEFAULT_BUFLEN];
                char name[DEFAULT_BUFLEN];
                strcpy(response, GAME_EVENT_NAME);
                std::cin.getline(name, DEFAULT_BUFLEN);

                strcat(response, name);
                send_message(s, response, DEFAULT_BUFLEN);
                return;
            }
            else if (strstr(buf, GAME_EVENT_START))
            {
                printf("Main host started the play!\n");
                write_formatted_log(GRAY "[CLIENT LOG] Changing state to ROLE\n" RESET);
                stage = STAGE_ROLE;
                return;
            }
            // Add case for loading card information
            else
            {
                write_formatted_log(GRAY "[CLIENT LOG] Unable to handle message\n" RESET);
                return;
            }

            break;
        }

        case STAGE_ROLE:
        {
             if (strstr(buf,GAME_EVENT_ROLE))
             {
                int t;
                sscanf(buf + strlen(GAME_EVENT_ROLE), "%d", &t);
                this->role = ROLE(t);

           //printf("My role is %d", this->role); borrar al final
              
              //To send the rol in string format
        switch (this->role) {
            case 0:
                printf(" Eres un villager. Su objetivo es eliminar a los werewolves.\n");
                break;
            case 1:
                printf("Eres un werewolf. Su objetivo es eliminar a los villagers sin ser atrapado.\n");
                break;
            case 2:
                printf("Eres un seer. Puede verificar el rol de un jugador cada noche.\n");
                break;
            case 3:
                printf("Eres es un witch. Puede verificar el rol de un jugador cada noche.\n");
                break;
            case 4:
                 printf("Eres es un hunter. Su objetivo es eliminar a los werewolves.\n");
                break;
            default:
                printf("Rol desconocido para el jugador.\n");
                break;

              }

                write_formatted_log(GRAY "[CLIENT LOG] Changing state to NIGHT\n" RESET);
                this->stage = STAGE_NIGHT;
             }
             else {//POR ESO SE IMPRIME FATAL
                printf("Fatal Error from STAGE ROLE: %s", buf);
                
             }
             break;


        }
        case STAGE_NIGHT:
        {
            if(strstr(buf, GAME_EVENT_ACTION_WEREWOLF))
          {
            //ESPERAR ENTRADA DE USUARIO (VOTO)
           // ENVIAR A SERVIDOR "VOTO LOBO" + VOTO
           return;
          }
          if (strstr(buf, GAME_EVENT_ACTION_SEER))
          {
            //EXTRAER RESPUESTA DESDE EL MENSAJE
            //MOSTRAR RESPUESTA
            return;
          }
          if (strstr(buf, GAME_EVENT_ACTION_WITCH))
          {
            // ESPERAR ENTRADA DE USUARIO (JUGADOR, ACCION)
            // ENVIAR A SERVIDOR "POCION BRUJA" + (JUGADOR, ACCION)
            return;
          }
         
          if(strstr(buf,GAME_EVENT_DAY))
          {
            // ?
            stage = STAGE_ROLE;
            return;
          }
          // si no error 
          else {
            printf("Fatal Error from STAGE NIGHT: %s", buf);
          }
          break;

    }
        case STAGE_DAY:
        {
         if(strstr(buf,GAME_EVENT_DAY))
         {
                //EXTRAER MUERTOS DESDE EL MENSAJE
                //MOSTRAR MUERTOS
                //SI ESTOY EN MUERTOS Y NO SOY CAZADOR
                stage = STAGE_VIEWER;
                return;
         }
         if(strstr(buf,GAME_EVENT_NEW))
         {
           // EXTRAER MUERTOS DESDE EL MENSAJE
           // MOSTRAR MUERTOS
           // SI ESTOY EN MUERTOS Y NO SOY CAZADOR
         //{
             stage = STAGE_VIEWER;
         //}
         }
         if(strstr(buf,GAME_EVENT_ACTION_HUNTER1))
         {
            //ESPERAR ENTRADA DE USUARIO (JUGADOR)
            //ENVIAR A SERVIDOR "VICTIMA CAZADOR 1" + JUGADOR 
            stage=STAGE_VIEWER;
            return;
         }
         if(strstr(buf,GAME_EVENT_ACTION_HUNTER2))
         {
           //ESPERAR ENTRADA DE USUARIO (JUGADOR)
           //ENVIAR A SERVIDOR "VICTIMA CAZADOR 2" + JUGADOR
           stage = STAGE_VIEWER;
            return;
         }
         if(strstr(buf,GAME_EVENT_VICTIM_HUNTER))
         {
              //EXTRAER VICTIMA DESDE MENSAJE
              //MOSTRAR VICTIMA
              //SI SOY VICTIMA
               stage = STAGE_VIEWER;
            return;
         }
         if(strstr(buf,GAME_EVENT_ACTION_EVERYONE))
         {
            //ESPERAR ENTRADA DE USUARIO (VOTO)
            //ENVIAR A SERVIDOR "VOTO JUGADOR" + VOTO
            return;
         }
         if(strstr(buf,GAME_EVENT_NIGHT))
         {
            stage = STAGE_NIGHT;
            return;
         }
         if(strstr(buf,GAME_EVENT_GAMEOVER))
         {
            //EXTRAER GANADOR DESDE EL MENSAJE
            //MOSTRAR GANADOR
            stage = STAGE_GAME_OVER;
            return; //fin del juego
         }
         else
         {
            printf("Fatal Error from STAGE DAY %s", buf);
        } 
    }

    }

}