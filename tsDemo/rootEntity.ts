import * as cpp from 'cpp';

export class RootEntity{
    private entities:Array<Entity> = new Array<Entity>();

    public tick():void{
        for(let entity of this.entities){
            switch(entity.lifeState){
                case 1:
                    entity.onMessage();
                    break;
                case 0:
                    entity.begin();
                    entity.id = cpp.EntityManager.Instance().CreateEntity(entity.entityType);
                    break;
                case 2:
                    entity.stop();
                    cpp.EntityManager.Instance().DestroyEntity(entity.id);
                    break;
                default:
                    break;
            }
        }               
    }
}

interface Entity{
    /**
     * 生命周期状态：1-待加载，2-加载完成，3-待释放
     */
    lifeState:number;

    /**
     * 实体类型
     */
    entityType:string;


    /**
     * 唯一标识
     */
    id:number;

    /**
     * 第一次加载完成时执行
     */
    begin():void;

    /**
     * 销毁时执行
     */
    stop():void;

    /**
     * 每帧触发
     */
    onMessage():void;
    
}