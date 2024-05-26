workspace {
    name "Сайт заказа услуг"
    description "Сайт для поиска, создания и получения разного рода услуг"

    # включаем режим с иерархической системой идентификаторов
    !identifiers hierarchical

    !docs documentation

    # Модель архитектуры
    model {

        # Настраиваем возможность создания вложенных груп
        properties {
            structurizr.groupSeparator "/"
        }


        # Описание компонент модели
        users = person "Пользователи сайта заказа услуг"
        ordering_services = softwareSystem "Сайт заказа услуг" {
            description "Сайт для поиска, создания и получения разного рода услуг"

            users_service = container "Users service" {
                description "Сервис управления пользователями"
            }

            orders_service = container "Orders service" {
                description "Сервис управления заказами"
            }

            services_service = container "Services service" {
                description "Сервис управления услугами"
            }

        group "Слой данных" {
                users_database = container "Users Database" {
                    description "База данных с пользователями"
                    technology "PostgreSQL 15"
                    tags "database"
                }

                users_cache = container "Users Cache" {
                    description "Кеш пользовательских данных для ускорения аутентификации"
                    technology "Redis 6"
                    tags "database"
                }

                orders_database = container "Orders Database" {
                    description "База данных с заказами"
                    technology "PostgreSQL 15"
                    tags "database"
                }

                services_database = container "Services Database" {
                    description "База данных с услугами"
                    technology "PostgreSQL 15"
                    tags "database"
                }
            }

            users_service -> users_cache "Получение/обновление данных о пользователях" "TCP 6379"
            users_service -> users_database "Получение/обновление данных о пользователях" "TCP 5432"

            orders_service -> orders_database "Получение/обновление данных о заказах" "TCP 5432"
            orders_service -> users_service "Аутентификация пользователя" "REST HTTP 443"

            services_service -> services_database "Получение/обновление данных о сервисах" "TCP 5432"
            services_service -> users_service "Аутентификация пользователя" "REST HTTP 443"

            users -> users_service "Регистрация нового пользователя" "REST HTTP:8080"
            users -> orders_service "Операции пользователя с заказами" "REST HTTP:8080"
            users -> services_service "Операции пользователя с услугами" "REST HTTP:8080"
        }

        users -> ordering_services "Создание и заказ услуг"

        deploymentEnvironment "Production" {
            deploymentNode "Users Server" {
                containerInstance ordering_services.users_service
            }

            deploymentNode "Orders Server" {
                containerInstance ordering_services.orders_service
                instances 3
            }

            deploymentNode "Services Server" {
                containerInstance ordering_services.services_service
                instances 3
            }

            deploymentNode "databases" {

                deploymentNode "Database Server 1" {
                    containerInstance ordering_services.users_database
                }

                deploymentNode "Database Server 2" {
                    containerInstance ordering_services.orders_database
                    instances 3
                }

                deploymentNode "Database Server 3" {
                    containerInstance ordering_services.services_database
                    instances 3
                }

                deploymentNode "Cache Server" {
                    containerInstance ordering_services.users_cache
                }
            }

        }
    }

    views {
        themes default

        properties {
            structurizr.tooltips true
        }


        !script groovy {
            workspace.views.createDefaultViews()
            workspace.views.views.findAll { it instanceof com.structurizr.view.ModelView }.each { it.enableAutomaticLayout() }
        }

        dynamic ordering_services "UC01" "Создание нового пользователя" {
            autoLayout
            users -> ordering_services.users_service "Создать нового пользователя (POST /user)"
            ordering_services.users_service -> ordering_services.users_database "Сохранить данные о пользователе"
        }

        dynamic ordering_services "UC02" "Поиск пользователя по логину" {
            autoLayout
            users -> ordering_services.users_service "Поиск пользователя по логину (GET /user)"
            ordering_services.users_service -> ordering_services.users_database "Поиск пользователя по логину"
        }

        dynamic ordering_services "UC03" "Поиск пользователя по маске имя и фамилии" {
            autoLayout
            users -> ordering_services.users_service "Поиск пользователя по маске имя и фамилии (GET /user)"
            ordering_services.users_service -> ordering_services.users_database "Поиск пользователя по маске имя и фамилии"
        }

        dynamic ordering_services "UC04" "Создание услуги" {
            autoLayout
            users -> ordering_services.services_service "Создать услугу (POST /service)"
            ordering_services.services_service -> ordering_services.users_service "Проверить аутентификацию пользователя (GET /user)"
            ordering_services.services_service -> ordering_services.services_database "Сохранить данные об услуге"
        }

        dynamic ordering_services "UC05" "Получение списка услуг" {
            autoLayout
            users -> ordering_services.services_service "Получение списка услуг (GET /services)"
            ordering_services.services_service -> ordering_services.services_database "Получение списка услуг"
        }

        dynamic ordering_services "UC06" "Добавление услуг в заказ" {
            autoLayout
            users -> ordering_services.orders_service "Добавить услугу в заказ (POST /order)"
            ordering_services.orders_service -> ordering_services.users_service "Проверить аутентификацию пользователя (GET /user)"
            ordering_services.orders_service -> ordering_services.orders_database "Сохранить данные о заказе"
        }

        dynamic ordering_services "UC07" "Получение заказа для пользователя" {
            autoLayout
            users -> ordering_services.orders_service "Получить заказ для пользователя (GET /order)"
            ordering_services.orders_service -> ordering_services.users_service "Проверить аутентификацию пользователя (GET /user)"
            ordering_services.orders_service -> ordering_services.orders_database "Получить данные о заказе"
        }


        styles {
            element "database" {
                shape cylinder
            }
        }
    }
}
