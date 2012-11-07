/**
 * @name MFT.AppSubMenuView
 * 
 * @desc Media App Options module visual representation
 * 
 * @category    View
 * @filesource  app/view/media/AppSubMenuView.js
 * @version     2.0
 *
 * @author      Andriy Melnik
 */
 
MFT.AppSubMenuView = Em.ContainerView.create({
    classNameBindings:  [ 'MFT.States.media.appsubmenu.active:active_state',
                          //  'MFT.MediaController.currentOptionsData.view',
                        ],

    classNames:        ['media_app_sub_menu_view', 'hidden'],

    elementId:          'media_app_sub_menu_view',
    childViews:         [
                            'backButton',
                            'buttonsWrapper'
                        ],

    backButton: MFT.Button.extend({
        classNames:        ['backButton','button'],     
        action:            'optionsBack',
        target:            'MFT.MediaController',   
        icon:              'images/media/ico_back.png',   
    }),

    PerformInteraction: function( interactionChoiceSetIDList ){

        for(var IDList in interactionChoiceSetIDList){

            for(var ChoisesVal in MFT.AppModel.interactionChoises){

                if( interactionChoiceSetIDList[IDList] == MFT.AppModel.interactionChoises[ChoisesVal].interactionChoiceSetID ){
                    
                    for(var ChoiseSet in MFT.AppModel.interactionChoises[ChoisesVal].choiceSet){
                        button = MFT.Button.create({
                            elementId:          'media_app_options_view' + MFT.AppModel.interactionChoises[ChoisesVal].choiceSet[ChoiseSet].menuName,
                            click:              function(){
                                FFW.UI.onCommand(MFT.AppModel.interactionChoises[ChoisesVal].choiceSet[ChoiseSet].choiceID);
                            },
                            commandId:          MFT.AppModel.interactionChoises[ChoisesVal].choiceSet[ChoiseSet].choiceID, 
                            classNames:         ['rs-item'],
                            //icon:             null,//'images/media/active_arrow.png',
                            text:               MFT.AppModel.interactionChoises[ChoisesVal].choiceSet[ChoiseSet].menuName 
                        });

                        MFT.AppSubMenuView.buttonsWrapper.buttonsScroll.get('childViews').pushObject(button);
                    }
                }
            }
        }

        MFT.AppSubMenuView.buttonsWrapper.scroll.refresh();
    },

    buttonsWrapper: Em.ContainerView.extend({
        classNames:         'buttonsWrapper',
        elementId:          'buttonsWrapper',
        childViews: [
            'buttonsScroll',
            'scrollBar'
        ],

        scroll:             null,

        pos:                0,

        scrollPos:          0,

        coeficient:         0,

        scrollBarH:         0,

        buttonsScroll: Em.ContainerView.extend( Ember.TargetActionSupport, {
            classNames: 'buttonsScroll',
            elementId:  'buttonsScroll',
            actionUp:   function(){
                this._parentView.pos = this._parentView.scroll.y - this._parentView.scroll.startY;
            }
        }),

        scrollBar: Em.ContainerView.extend({
            classNames:         'scrollBar',
            elementId:          'scrollBar',
            childViews: [
                'scrollArrowUp',
                'scroller',
                'scrollArrowDown'
            ],

            scrollArrowUp:   MFT.Button.extend({
                classNames: 'scrollArrows scrollArrowUp button',
                click:      function(){
                    MFT.AppSubMenuView.buttonsWrapper.scroll.scrollTo(0, MFT.AppSubMenuView.buttonsWrapper.scroll.y + 52, 200);
                    if( MFT.AppSubMenuView.buttonsWrapper.scroll.y < -52){
                        MFT.AppSubMenuView.buttonsWrapper.set('scrollPos', MFT.AppSubMenuView.buttonsWrapper.scroll.y + 52);
                    }else{
                        MFT.AppSubMenuView.buttonsWrapper.set('scrollPos', 0);
                    }
                }
            }),

            scroller: Em.View.extend({
                classNames: 'scroller',
                attributeBindings: ['style'],
                style: 'margin-top: 0px'
            }),

            scrollArrowDown:   MFT.Button.extend({
                classNames: 'scrollArrows scrollArrowDown button',
                click:      function(){
                    MFT.AppSubMenuView.buttonsWrapper.scroll.scrollTo(0, MFT.AppSubMenuView.buttonsWrapper.scroll.y - 52, 200);
                    if( Math.abs(MFT.AppSubMenuView.buttonsWrapper.scroll.y) < ((MFT.AppSubMenuView.buttonsWrapper.scroll.scrollerH - MFT.AppSubMenuView.buttonsWrapper.scroll.wrapperH) - 52) ){
                        MFT.AppSubMenuView.buttonsWrapper.set('scrollPos', MFT.AppSubMenuView.buttonsWrapper.scroll.y - 52);
                    }else{
                        MFT.AppSubMenuView.buttonsWrapper.set('scrollPos',  (MFT.AppSubMenuView.buttonsWrapper.scroll.scrollerH - MFT.AppSubMenuView.buttonsWrapper.scroll.wrapperH));
                    }
                }
            }),
            
            moveScrollBar:  function(){
                
                this.scroller.set( 'style', 'height:' + this._parentView.scrollBarH + 'px; margin-top:' + Math.abs(this._parentView.scrollPos) * this._parentView.coeficient + 'px;' );
            }.observes('this.parentView.scrollPos'),
        }),
        
        scrollEnd:  function(){
            if( MFT.AppSubMenuView.buttonsWrapper.scroll.y > 0){
                this.set('scrollPos', 0);
            }else if( Math.abs(MFT.AppSubMenuView.buttonsWrapper.scroll.y) > (MFT.AppSubMenuView.buttonsWrapper.scroll.scrollerH - MFT.AppSubMenuView.buttonsWrapper.scroll.wrapperH) ){
                this.set('scrollPos', MFT.AppSubMenuView.buttonsWrapper.scroll.scrollerH - MFT.AppSubMenuView.buttonsWrapper.scroll.wrapperH);
            }else{
                this.set('scrollPos', MFT.AppSubMenuView.buttonsWrapper.scroll.y);
            }
        },
        
        loaded: function() {
            this.set('scroll', new iScroll('buttonsWrapper',{scrollbarClass: 'display:none', momentum: false, onBeforeScrollEnd: function(){MFT.AppSubMenuView.buttonsWrapper.scrollEnd();}}));
            this.coeficient = this.scroll.wrapperH / this.scroll.scrollerH;
            this.scrollBarH = (this.scroll.wrapperH - 98) * this.coeficient - 49;
            this.scrollBar.scroller.set( 'style', 'height:' + this.scrollBarH + 'px;' );

            button = MFT.Button.create({
                elementId:          'media_app_options_view',
                click:              function(){
                    MFT.MediaController.turnOnAppSubMenu(1);
                },
                commandId:          1, 
                classNames:         ['rs-item'],
                //icon:             null,//'images/media/active_arrow.png',
                text:               "menuName" 
            });

            MFT.AppSubMenuView.buttonsWrapper.buttonsScroll.get('childViews').pushObject(button);
            MFT.AppSubMenuView.buttonsWrapper.scroll.refresh();
        }
    }),

    afterRender: function() {
/*
        var butt;

        for(var i in MFT.InfoController.get('appsParams')){

            butt = MFT.Button.create({
                goToState:          MFT.InfoController.get('appsParams')[i].goToState,
                classNames:         MFT.InfoController.get('appsParams')[i].classNames,
                icon:               MFT.InfoController.get('appsParams')[i].icon,
                textBinding:        MFT.InfoController.get('appsParams')[i].textBinding,
                arrow:              MFT.InfoController.get('appsParams')[i].arrow,
                action:             MFT.InfoController.get('appsParams')[i].action,
                target:             MFT.InfoController.get('appsParams')[i].target,
                disabledBinding:    MFT.InfoController.get('appsParams')[i].disabledBinding,
                onDown:             MFT.InfoController.get('appsParams')[i].onDown
            });

            MFT.AppSubMenuView.buttonsWrapper.buttonsScroll.get('_childViews').pushObject(butt);
        }
*/
        setTimeout(function () { MFT.AppSubMenuView.buttonsWrapper.loaded(); }, 200);
        
    }
});